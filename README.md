# UE5 Third Person Shooter

![Gameplay Screenshot](Images/gameplay.png)

A third-person shooter game built with **Unreal Engine 5.6** and **C++**. The project features a player-controlled character equipped with a hitscan weapon, AI enemies powered by Behavior Trees and Blackboards, a real-time HUD, Niagara visual effects, and mobile-ready input support. The project is designed to showcase gameplay programming, AI systems, UI development, and Unreal Engine architecture using modern C++.

---

## Table of Contents

* [Features](#features)
* [Project Structure](#project-structure)
* [Architecture Overview](#architecture-overview)
* [C++ Source Classes](#c-source-classes)

  * [Core](#core)
  * [Player](#player)
  * [Weapon](#weapon)
  * [AI System](#ai-system)
  * [Behavior Tree Nodes](#behavior-tree-nodes)
  * [HUD & UI](#hud--ui)
* [Requirements](#requirements)
* [License](#license)

---

## Features

* **Third-Person Camera** — Spring arm + follow camera with full orbit control via mouse/gamepad
* **Enhanced Input System** — Move, Look, Jump, and Shoot actions using UE5's Enhanced Input
* **Mobile Support** — Conditional touch controls and separate Input Mapping Contexts for mobile platforms
* **Weapon System** — Skeletal mesh gun attached to a socket; hitscan line-trace with configurable range and damage
* **Niagara VFX** — Muzzle flash and impact particle effects via Niagara
* **Sound** — Shoot and impact sounds using GameplayStatics
* **AI Enemies** — Custom AIController powered by Behavior Trees and Blackboards
* **Line-of-Sight AI** — Enemy detection based on visibility checks
* **AI Shooting** — Enemies fire using the same weapon interface as the player
* **Health System** — Damage handling using UE's damage framework
* **HUD** — Real-time UMG health bar updates
* **Game Mode** — Automatic AI initialization and Behavior Tree startup

---

## Project Structure

```text
UE_5_ThirdPersonShooter/
├── Config/
├── Content/
├── Source/
│   ├── Shooter.Target.cs
│   ├── ShooterEditor.Target.cs
│   └── Shooter/
│       ├── Shooter.h / .cpp
│       ├── Shooter.Build.cs
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

```text
AShooterGameMode
  └── BeginPlay()
        └── Finds all AShooterAI in world
              └── Calls AShooterAI::StartBehaviorTree(Player)
                    └── Runs EnemyAIBehaviorTree
                          ├── BTService_PlayerLocation
                          ├── BTService_PlayerLocationIfSeen
                          ├── BTTaskNode_Shoot
                          └── BTTask_ClearBlackboardValue

AShooterPlayerController
  ├── Sets up Enhanced Input Mapping Contexts
  ├── Spawns mobile touch controls
  └── Creates HUD widget

AShooterCharacter
  ├── Camera System
  ├── Weapon System
  ├── Input Handling
  ├── Health System
  └── HUD Updates

AGun
  ├── Hitscan Weapon Logic
  ├── Niagara Effects
  ├── Audio Feedback
  └── Damage Application
```

---

## C++ Source Classes

### Core

#### `Shooter.h` / `Shooter.cpp`

Module entry point. Registers the Shooter module and defines the global logging category used throughout the project.

---

### Player

#### `AShooterCharacter` — `ShooterCharacter.h / .cpp`

Abstract base class for both player-controlled and AI-controlled characters.

| Member                                                  | Description                            |
| ------------------------------------------------------- | -------------------------------------- |
| `CameraBoom`                                            | Spring arm used to position the camera |
| `FollowCamera`                                          | Third-person camera                    |
| `JumpAction`, `MoveAction`, `LookAction`, `ShootAction` | Enhanced Input actions                 |
| `GunClass`                                              | Blueprint-assigned weapon class        |
| `Gun`                                                   | Spawned weapon instance                |
| `MaxHealth`                                             | Maximum character health               |
| `Health`                                                | Current health value                   |
| `IsAlive`                                               | Character life state                   |
| `Shoot()`                                               | Fires the equipped weapon              |
| `OnDamageTaken()`                                       | Handles incoming damage                |
| `UpdateHUD()`                                           | Updates health bar values              |

Movement defaults:

* MaxWalkSpeed = 500
* JumpZVelocity = 500
* AirControl = 0.35
* RotationRate = 500 yaw

#### `AShooterPlayerController` — `ShooterPlayerController.h / .cpp`

Handles input initialization, mobile controls, and HUD creation.

#### `AShooterGameMode` — `ShooterGameMode.h / .cpp`

Initializes enemy AI and starts Behavior Trees at game startup.

---

### Weapon

![Weapon & Combat](Images/gameplay.png)

#### `AGun` — `Gun.h / .cpp`

Standalone weapon actor attached to the player's weapon socket.

Key responsibilities:

* Hitscan shooting
* Niagara muzzle flash effects
* Impact particle effects
* Shooting audio
* Impact audio
* Damage application

`PullTrigger()`:

1. Activates muzzle flash
2. Plays shoot sound
3. Gets controller viewpoint
4. Performs line trace
5. Applies damage and impact effects

---

### AI System

![Behavior Tree](Images/ai-behavior-tree.png)

#### `AShooterAI` — `ShooterAI.h / .cpp`

Custom AI Controller responsible for running enemy Behavior Trees.

Responsibilities:

* Maintains player references
* Maintains controlled pawn references
* Initializes Behavior Trees
* Updates Blackboard values
* Controls enemy combat behavior

---

### Behavior Tree Nodes

#### `UBTService_PlayerLocation`

Updates player position in the Blackboard every tick.

#### `UBTService_PlayerLocationIfSeen`

Updates Blackboard values only when the player is visible.

#### `UBTTaskNode_Shoot`

Triggers enemy shooting behavior.

#### `UBTTask_ClearBlackboardValue`

Clears Blackboard keys when required.

---

### HUD & UI

![HUD](Images/hud.png)

#### `UHUDWidget` — `HUDWidget.h / .cpp`

UMG widget responsible for displaying player health.

Contains:

* Health bar widget
* Health percentage updates
* Runtime validation before UI updates

---

## Requirements

| Requirement       | Version |
| ----------------- | ------- |
| Unreal Engine     | 5.6     |
| Language Standard | C++17   |
| Target Platforms  | Windows |
| License           | MIT     |

---

## License

MIT License — Copyright © 2026 Shaurya Goyal. See `License.txt` for the full license text.
