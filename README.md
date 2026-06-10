# UE5 Third Person Shooter

A third-person shooter game built with **Unreal Engine 5.6** and **C++**. The project features a player-controlled character with a gun, a full AI enemy system driven by Behavior Trees, a live HUD, and mobile-ready input support. Currently under active development.

---

## Table of Contents

- [Features](#features)
- [Project Structure](#project-structure)
- [Architecture Overview](#architecture-overview)
- [C++ Source Classes](#c-source-classes)
  - [Core](#core)
  - [Player](#player)
  - [Weapon](#weapon)
  - [AI System](#ai-system)
  - [Behavior Tree Nodes](#behavior-tree-nodes)
  - [HUD & UI](#hud--ui)
- [Module Dependencies](#module-dependencies)
- [Getting Started](#getting-started)
- [Requirements](#requirements)
- [License](#license)

---

## Features

- **Third-Person Camera** — Spring arm + follow camera with full orbit control via mouse/gamepad
- **Enhanced Input System** — Move, Look, Jump, and Shoot actions using UE5's Enhanced Input
- **Mobile Support** — Conditional touch controls and separate Input Mapping Contexts for mobile platforms
- **Weapon System** — Skeletal mesh gun attached to a socket; hitscan line-trace with configurable range and damage
- **Niagara VFX** — Muzzle flash and impact particle effects via `NiagaraComponent` / `NiagaraFunctionLibrary`
- **Sound** — Shoot and impact sounds via `UAudioComponent`/`GameplayStatics`
- **AI Enemies** — `AShooterAI` (AIController) drives enemies via a Behavior Tree initialized at game start
- **Line-of-Sight AI** — `BTService_PlayerLocationIfSeen` updates the Blackboard only when the player is visible; AI focuses/clears focus accordingly
- **AI Shooting** — `BTTaskNode_Shoot` fires the enemy's gun through the shared `AShooterCharacter::Shoot()` interface
- **Blackboard Integration** — Custom BT services/tasks for player tracking, LOS gating, and value clearing
- **Health System** — Float health with `OnTakeAnyDamage` delegate; death triggers collision disable + controller detach
- **HUD** — UMG `ProgressBar`-based health bar updated in real time via `UHUDWidget`
- **Game Mode** — `AShooterGameMode` auto-discovers all `AShooterAI` actors on `BeginPlay` and starts their Behavior Trees

---

## Project Structure

```
UE_5_ThirdPersonShooter/
├── Config/                        # DefaultEngine, DefaultGame, DefaultInput configs
├── Content/                       # Blueprints, Maps, Meshes, Animations, Materials (uassets)
├── Source/
│   ├── Shooter.Target.cs          # Game target build rules
│   ├── ShooterEditor.Target.cs    # Editor target build rules
│   └── Shooter/
│       ├── Shooter.h / .cpp       # Module entry point + LogShooter category
│       ├── Shooter.Build.cs       # UBT module rules & dependencies
│       ├── ShooterCharacter.h / .cpp
│       ├── ShooterGameMode.h / .cpp
│       ├── ShooterPlayerController.h / .cpp
│       ├── ShooterAI.h / .cpp
│       ├── Gun.h / .cpp
│       ├── HUDWidget.h / .cpp
│       ├── BTService_PlayerLocation.h / .cpp
│       ├── BTService_PlayerLocationIfSeen.h / .cpp
│       ├── BTTaskNode_Shoot.h / .cpp
│       └── BTTask_ClearBlackboardValue.h / .cpp
├── .gitignore
├── .gitattributes
├── License.txt
└── Shooter.uproject
```

---

## Architecture Overview

```
AShooterGameMode
  └── BeginPlay()
        └── Finds all AShooterAI in world
              └── Calls AShooterAI::StartBehaviorTree(Player)
                    └── Runs EnemyAIBehaviorTree
                          ├── BTService_PlayerLocation         (always tracks player position)
                          ├── BTService_PlayerLocationIfSeen   (LOS-gated position + focus)
                          ├── BTTaskNode_Shoot                 (fires via AShooterCharacter::Shoot)
                          └── BTTask_ClearBlackboardValue      (clears stale Blackboard keys)

AShooterPlayerController
  ├── Sets up Enhanced Input Mapping Contexts
  ├── Spawns mobile touch controls (if on mobile)
  └── Creates & adds UHUDWidget to viewport

AShooterCharacter  (abstract base — subclassed in Blueprint)
  ├── SpringArmComponent + CameraComponent
  ├── Owns AGun (spawned & socket-attached at BeginPlay)
  ├── Handles Move / Look / Jump / Shoot inputs
  ├── Health system with OnTakeAnyDamage
  └── Calls PlayerController → HUDWidget::SetHealthBarPercent()

AGun
  ├── SkeletalMeshComponent + NiagaraComponent (muzzle flash)
  ├── PullTrigger(): viewpoint line trace on ECC_GameTraceChannel2
  ├── Spawns Niagara impact VFX + plays impact sound on hit
  └── Applies damage via UGameplayStatics::ApplyDamage()
```

---

## C++ Source Classes

### Core

#### `Shooter.h` / `Shooter.cpp`
Module entry point. Registers the `Shooter` module with UE's module manager and declares the global `LogShooter` log category used throughout the project.

---

### Player

#### `AShooterCharacter` — `ShooterCharacter.h / .cpp`
Abstract base class for both the player and AI-controlled characters (inherits `ACharacter`).

| Member | Description |
|---|---|
| `CameraBoom` | `USpringArmComponent` — positions the camera behind the character |
| `FollowCamera` | `UCameraComponent` — attached to the boom end |
| `JumpAction`, `MoveAction`, `LookAction`, `MouseLookAction`, `ShootAction` | Enhanced Input `UInputAction` bindings |
| `GunClass` | Blueprint-assignable `TSubclassOf<AGun>` — spawned on BeginPlay |
| `Gun` | Runtime pointer to the spawned `AGun` instance |
| `MaxHealth` | Configurable max health (default `100.0`) |
| `Health` | Current health — updated on damage |
| `IsAlive` | Blueprint-readable bool — set false on death |
| `Shoot()` | Calls `Gun->PullTrigger()` |
| `OnDamageTaken(...)` | Bound to `OnTakeAnyDamage`; reduces health, updates HUD, triggers death logic |
| `UpdateHUD()` | Casts to `AShooterPlayerController` and sets health bar percent |
| `DoMove()`, `DoLook()`, `DoJumpStart()`, `DoJumpEnd()` | `BlueprintCallable` input handlers — callable from both C++ input bindings and mobile UI widgets |

Movement defaults: `MaxWalkSpeed=500`, `JumpZVelocity=500`, `AirControl=0.35`, `RotationRate=500 yaw`.

#### `AShooterPlayerController` — `ShooterPlayerController.h / .cpp`
Abstract `APlayerController` subclass. Sets up input and UI.

| Member | Description |
|---|---|
| `DefaultMappingContexts` | Array of `UInputMappingContext*` applied to all platforms |
| `MobileExcludedMappingContexts` | IMCs skipped when running on touch devices |
| `MobileControlsWidgetClass` | Widget class spawned on mobile |
| `HUDWidgetClass` | Blueprint-assignable `UHUDWidget` subclass |
| `HUDWidget` | Runtime pointer to the created HUD widget |

On `BeginPlay`: spawns mobile touch widget if `SVirtualJoystick::ShouldDisplayTouchInterface()`, then always creates and adds `HUDWidget` to the viewport.

#### `AShooterGameMode` — `ShooterGameMode.h / .cpp`
Abstract `AGameModeBase` subclass. On `BeginPlay`, gets the player pawn and iterates all `AShooterAI` actors in the world, calling `StartBehaviorTree(Player)` on each.

---

### Weapon

#### `AGun` — `Gun.h / .cpp`
Standalone `AActor` — spawned by `AShooterCharacter` and attached to the `WeaponSocket` bone.

| Member | Description |
|---|---|
| `Mesh` | `USkeletalMeshComponent` — the gun mesh |
| `MuzzleFlashParticleSystem` | `UNiagaraComponent` — activated on each shot |
| `ImpactParticleSystem` | `UNiagaraSystem*` — spawned at hit point |
| `MaxRange` | Hitscan trace length (default `10000` units) |
| `BulletDamage` | Damage applied per shot (default `10.0`) |
| `ShootSound` | Played at gun location on fire |
| `ImpactSound` | Played at hit point on contact |
| `OwnerController` | Set by `AShooterCharacter`; used to get the viewpoint for the trace |

**`PullTrigger()`** flow:
1. Activates muzzle flash Niagara component
2. Plays shoot sound
3. Gets viewpoint from `OwnerController`
4. Runs `LineTraceSingleByChannel` on `ECC_GameTraceChannel2` (ignores self and owner)
5. On hit: spawns impact VFX, plays impact sound, calls `UGameplayStatics::ApplyDamage()`

---

### AI System

#### `AShooterAI` — `ShooterAI.h / .cpp`
Custom `AAIController`. Holds references to the player and the AI's own pawn character.

| Member | Description |
|---|---|
| `EnemyAIBehaviorTree` | Blueprint-assignable `UBehaviorTree*` |
| `PlayerCharacter` | Pointer to the player's `AShooterCharacter` |
| `MyCharacter` | Pointer to the AI's own `AShooterCharacter` pawn |
| `StartBehaviorTree(Player)` | Casts possessed pawn, stores player ref, calls `RunBehaviorTree()`, and writes `StartLocation` to the Blackboard |

---

### Behavior Tree Nodes

#### `UBTService_PlayerLocation` — `BTService_PlayerLocation.h / .cpp`
BT Service (extends `UBTService_BlackboardBase`). On every tick, unconditionally writes the player pawn's world location to the selected Blackboard key. Used for patrol/chase when LOS is not required.

#### `UBTService_PlayerLocationIfSeen` — `BTService_PlayerLocationIfSeen.h / .cpp`
BT Service. On every tick, checks `LineOfSightTo(Player)`. If the player is visible:
- Writes player location to the Blackboard key
- Calls `SetFocus(Player)` on the AI controller

If the player is **not** visible:
- Clears the Blackboard key
- Calls `ClearFocus(EAIFocusPriority::Gameplay)`

#### `UBTTaskNode_Shoot` — `BTTaskNode_Shoot.h / .cpp`
BT Task. Executes the AI's shoot action by calling `OwnerCharacter->Shoot()` (which in turn calls `Gun->PullTrigger()`). Returns `Succeeded` if both the owner and the player are valid and the player is alive; otherwise returns `Failed`.

#### `UBTTask_ClearBlackboardValue` — `BTTask_ClearBlackboardValue.h / .cpp`
BT Task (extends `UBTTask_BlackboardBase`). Clears any Blackboard key selected in the editor. Always returns `Succeeded`. Useful for resetting LOS/location state when switching BT branches.

---

### HUD & UI

#### `UHUDWidget` — `HUDWidget.h / .cpp`
UMG widget (`UUserWidget` subclass). Contains a `UProgressBar* HealthBar` bound via `BindWidgetOptional`. `SetHealthBarPercent(float)` validates the range `[0, 1]` before calling `HealthBar->SetPercent()`.

---

## Module Dependencies

Declared in `Shooter.Build.cs`:

| Module | Purpose |
|---|---|
| `Core`, `CoreUObject`, `Engine` | UE5 runtime fundamentals |
| `InputCore`, `EnhancedInput` | New Enhanced Input system |
| `AIModule` | `AAIController`, `UBehaviorTree`, `UBlackboardComponent` |
| `StateTreeModule`, `GameplayStateTreeModule` | Epic's StateTree AI framework (available for future use) |
| `UMG`, `Slate` | Widget system for the HUD |
| `Niagara` | Particle VFX for muzzle flash and impact effects |

The build file also declares include paths for several planned sub-variants: `Variant_Platforming`, `Variant_Combat`, and `Variant_SideScrolling` — indicating future gameplay mode expansions.

---

## Getting Started

### Prerequisites

- **Unreal Engine 5.6** (set in `Shooter.uproject` via `"EngineAssociation": "5.6"`)
- **Visual Studio 2022** (Windows) or **Xcode** (macOS) with the C++ game development workload
- Git with LFS (for binary assets)

### Setup

```bash
# Clone the repository
git clone https://github.com/Shaurya1907/UE_5_ThirdPersonShooter.git
cd UE_5_ThirdPersonShooter

# Right-click Shooter.uproject → "Generate Visual Studio project files"
# Then open Shooter.sln and build the Development Editor configuration

# Or launch directly:
# Double-click Shooter.uproject (requires UE 5.6 installed)
```

### Blueprint Setup Required

Because `AShooterCharacter`, `AShooterPlayerController`, and `AShooterGameMode` are all declared `abstract` in C++, you must create Blueprint subclasses and assign:

- **ShooterCharacter BP** — skeletal mesh, anim blueprint, `GunClass`, `MaxHealth`, and all `UInputAction` assets
- **ShooterPlayerController BP** — `HUDWidgetClass`, `DefaultMappingContexts`, `MobileControlsWidgetClass`
- **ShooterGameMode BP** — set as the world's Game Mode; it will auto-start AI behavior trees
- **Gun BP** — skeletal mesh, muzzle flash Niagara asset, `ImpactParticleSystem`, `ShootSound`, `ImpactSound`, `BulletDamage`, `MaxRange`
- **Enemy BP** — uses `AShooterCharacter` base with `AShooterAI` as its AI Controller class; requires an assigned `EnemyAIBehaviorTree`

---

## Requirements

| Requirement | Version |
|---|---|
| Unreal Engine | 5.6 |
| Language Standard | C++17 (UE default) |
| Target Platforms | Windows, macOS, iOS, Android (mobile input support present) |
| License | MIT |

---

## License

MIT License — Copyright © 2026 Shaurya Goyal. See [License.txt](License.txt) for the full text.