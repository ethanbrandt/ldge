# Project Goals
LDGE is a simple 2D game engine written in C/C++, when creating the project we wanted to:
* Create a simple to use scripting interface
* Modularize codebase to allow for easy collaboration
* Build low-level understanding of game engine architecture

We avoided:
* Complex UI
* Native C++ Scripting

---

# Contributers
**Daniel Ribuffo**
* Project Manager
* Scrum Master
* Audio System Engineer
* Input System Engineer
<br></br>

**Ethan Brandt**
* Product Owner
* Lead Developer
* Scripting System Engineer
* File Handling System Engineer
* Entity Compenent System Engineer
* Rendering System Engineer
<br></br>

**Aaron Richard**
* Physics System Engineer
<br></br>

**Asher Frost**
* Rendering System Engineer
<br></br>

*Special thanks to **Anthony Van Weel** & **Jen Barnes** for their help and support*
<br></br>

---

# Lua Scripting API Documentation

## Lifecycle Callbacks
These are Lua functions you **define** in your script. The engine will call them automatically if they exist.

**start**(*entityId*)  
  Called once when the entity is created and its script is loaded.

**update**(*entityId, deltaTime*)  
  Called every frame for the entity, with *deltaTime* in seconds since the last frame.

**on_trigger**(*entityId, otherEntityId*)  
  Called when this entity’s collider enters a trigger interaction with *otherEntityId*.

**on_collision**(*entityId, otherEntityId*)  
  Called when this entity collides (non-trigger) with *otherEntityId*.

**on_destroy**(*entityId*)  
  Called just before the entity is destroyed.

**on_message_received**(*entityId, senderEntityId, msg*)  
  Called when this entity receives a message from another entity via **SendMessage**.

---

## Log Interfaces

**Log**(*msg*)  
  Outputs *msg* to the console as a standard log message.

**LogError**(*msg*)  
  Outputs *msg* to the console as an error log.



## File / Asset Interfaces

**LoadAudioFromFile**(*filePath*)  
  Loads an audio file at *filePath* (.json) into the audio system so it can be played.

**LoadFontFromFile**(*filePath*)  
  Loads a font definition file at *filePath* (.json) for use by the rendering/UI system.

**LoadEntityFromFile**(*filePath, x, y*) → *entityId*  
  Spawns a new entity from the data at *filePath* (.json) at world position (*x*, *y*), and returns its *entityId*.



## Rigidbody / Physics Interfaces

**SetPosition**(*entityId, x, y*)  
  Sets the world position of the entity's rigidbody to (*x*, *y*).

**GetPosition**(*entityId*) -> *x, y*  
  Returns the world position (*x*, *y*) of the entity's rigidbody, or `nil, nil` if not found.

**SetVelocity**(*entityId, vx, vy*)  
  Sets the velocity of the entity's rigidbody to (*vx*, *vy*).

**GetVelocity**(*entityId*) -> *vx, vy*  
  Returns the velocity (*vx*, *vy*) of the entity's rigidbody, or `nil, nil` if not found.

**IsTrigger**(*entityId*) -> *bool*  
  Returns `true` if the entity's collider is a trigger, `false` if not, or `nil` if not found.

**IsStatic**(*entityId*) -> *bool*  
  Returns `true` if the entity's rigidbody is static (non-moving), `false` if dynamic, or `nil` if not found.



## Actor / Rendering Interfaces

**SetSprite**(*entityId, spritePath*)  
  Sets the entity's sprite to the image at *spritePath* (.bmp).

**SetSpriteSheet**(*entityId, spritePath, spriteWidth, spriteHeight*)  
  Sets a sprite sheet for the entity's sprite from *spritePath* (.bmp) with each frame sized *spriteWidth × spriteHeight*.

**SetSpriteIndex**(*entityId, indexX, indexY*)  
  Selects which cell of the sprite sheet to display using the indices (*indexX*, *indexY*).

**SetScreenScale**(*entityId, scaleX, scaleY*)  
  Sets the visual scale of the entity's sprite on screen to (*scaleX*, *scaleY*).



## Audio Interfaces

**PlaySound**(*audioPath*)  
  Plays the sound identified by *audioPath* (.wav) (which must have been loaded).

**SetVolume**(*audioPath, volume*)  
  Sets the playback volume for the sound at *audioPath* (.wav) to *volume* (clamped from 0.0-1.0).

**PauseAllAudio**()  
  Pauses all currently playing audio.

**ResumeAllAudio**()  
  Resumes all audio that was paused.



## Input Interfaces

**IsPressed**(*key*) -> *bool*  
  Returns `true` if the given key is currently held down.  
  *key* is a one-character string (e.g. `"A"`).

**WasPressedDown**(*key*) -> *bool*  
  Returns `true` on the frame where the given key transitions from up to down.

**WasReleased**(*key*) -> *bool*  
  Returns `true` on the frame where the given key transitions from down to up.



## Control / Messaging Interfaces

**DestroyEntity**(*entityId*)  
  Destroys the entity with the given *entityId*, triggering its **on_destroy** callback if defined.

**SendMessage**(*entityId, receiverEntityId, msg*)  
  Sends the string *msg* from *entityId* to *receiverEntityId*, triggering **on_message_received** on the receiver if defined.

---

# JSON File Formats

This document describes the JSON formats used by the engine’s `FileHandler` for loading audio, fonts, entities, and scenes. Each section lists required/optional fields and gives example JSON files.

There **must** be a scene named `start_scene.json` for game initialization.

## Audio Config (`LoadAudioFromFile`)

**Used by:**
`LoadAudioFromFile(path)`
`LoadAudioFromFile` is also used indirectly by scenes via `songPath`.

### Schema

Top-level object:

* **`audioFilePath`** — `string`
  Path to the actual audio file (e.g. `.wav`, `.ogg`, etc.).
* **`startVolume`** — `number` (`float`)
  Initial volume (e.g. `0.0`–`1.0`).
* **`isLooping`** — `boolean`

  * `true` → added as a **song** (looping music).
  * `false` → added as a **sound** (one-shot / SFX).

All three keys are **required**. If any is missing, the file is rejected and an error is logged.

### Example

```json
{
  "audioFilePath": "assets/audio/music/overworld_theme.ogg",
  "startVolume": 0.7,
  "isLooping": true
}
```

```json
{
  "audioFilePath": "assets/audio/sfx/jump.wav",
  "startVolume": 1.0,
  "isLooping": false
}
```



## Font Config (`LoadFontFromFile`)

**Used by:**
`LoadFontFromFile(filePath)`

### Schema

Top-level object:

* **`fontAtlasFilePath`** — `string`
  Path to the font atlas image.
* **`fontAtlasOrder`** — `string`
  Characters in the order they appear in the atlas (row-major).
* **`charWidth`** — `integer`
  Width of each character in pixels.
* **`charHeight`** — `integer`
  Height of each character in pixels.
* **`horizontalPadding`** — `integer`
  Horizontal spacing (in pixels) between characters in the atlas.

All fields are **required**. Missing keys cause the file to be rejected.

### Example

```json
{
  "fontAtlasFilePath": "assets/fonts/retro_font.png",
  "fontAtlasOrder": "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.!?- ",
  "charWidth": 8,
  "charHeight": 8,
  "horizontalPadding": 1
}
```



## Entity Config (`LoadEntityFromFile`)

**Used by:**
`LoadEntityFromFile(filePath, startPos)`

### Overview

An entity JSON describes:

* Which **script** to attach.
* How to build its **rigidbody** (mass / static / collision shape).
* How to build its **actor** (sprite or sprite sheet, scale, start frame).

If this file is invalid (missing required keys or invalid collision shape), the entity will **not** be created.

### Top-Level Schema

Top-level keys:

* **`scriptFilePath`** — `string` (**required**)
  Path to the Lua script for this entity.

* **`rigidBody`** — `object` (**required**)
  See **Rigidbody Schema**.

* **One of:**

  * **`sprite`** — `object`
  * **`spriteSheet`** — `object`

If neither `sprite` nor `spriteSheet` exists, the file is rejected.

#### Rigidbody Schema (`rigidBody`)

* **`mass`** — `number` (`float`, optional)
  Defaults to `1.0` if omitted.
* **`isStatic`** — `boolean` (optional)
  Defaults to `false` if omitted.

Exactly one of:

* **`collisionCircle`** — `object`
* **`collisionRectangle`** — `object`

If neither is present, this is an error.

##### `collisionCircle`

* **`radius`** — `number` (**required**)
* **`offset`** — `array[number, number]` (optional)
  Local offset of the collider center from the rigidbody position. Defaults to `[0, 0]`.
* **`isTrigger`** — `boolean` (optional)
  Defaults to `false`.
* **`collisionMask`** — `integer` (optional)
  Defaults to `0`.

##### `collisionRectangle`

* **`width`** — `number` (**required**)
* **`height`** — `number` (**required**)
* **`offset`** — `array[number, number]` (optional)
  Local offset of the rectangle center. Defaults to `[0, 0]`.
* **`isTrigger`** — `boolean` (optional)
  Defaults to `false`.
* **`collisionMask`** — `integer` (optional)
  Defaults to `0`.

#### Actor Schema (`sprite` or `spriteSheet`)

You must use **either** `sprite` **or** `spriteSheet`.

##### `sprite`

* **`spriteFilePath`** — `string` (**required**)
  Path to the image file.
* **`scale`** — `array[number, number]` (optional)
  Visual scale of the sprite. Defaults to `[1.0, 1.0]` if missing.

##### `spriteSheet`

* **`spriteFilePath`** — `string` (**required**)
  Path to the sprite sheet image.
* **`spriteWidth`** — `number` (**required**)
  Width of a single frame, in pixels.
* **`spriteHeight`** — `number` (**required**)
  Height of a single frame, in pixels.
* **`scale`** — `array[number, number]` (optional)
  Visual scale. Defaults to `[1.0, 1.0]`.
* **`startSpriteIndex`** — `array[integer, integer]` (optional)
  Starting frame index (`[xIndex, yIndex]`) in the sheet. Defaults to `[0, 0]`.

### Example: Entity with Circle Collider & Simple Sprite

```json
{
  "scriptFilePath": "assets/scripts/player.lua",

  "rigidBody": {
    "mass": 1.0,
    "isStatic": false,

    "collisionCircle": {
      "radius": 0.5,
      "offset": [0.0, -0.2],
      "isTrigger": false,
      "collisionMask": 1
    }
  },

  "sprite": {
    "spriteFilePath": "assets/sprites/player.png",
    "scale": [2.0, 2.0]
  }
}
```

### Example: Entity with Rectangle Collider & Sprite Sheet

```json
{
  "scriptFilePath": "assets/scripts/slime.lua",

  "rigidBody": {
    "mass": 0.5,
    "isStatic": false,

    "collisionRectangle": {
      "width": 1.0,
      "height": 0.6,
      "offset": [0.0, -0.1],
      "isTrigger": false,
      "collisionMask": 2
    }
  },

  "spriteSheet": {
    "spriteFilePath": "assets/sprites/slime_sheet.png",
    "spriteWidth": 16,
    "spriteHeight": 16,
    "scale": [1.5, 1.5],
    "startSpriteIndex": [0, 0]
  }
}
```

> Note: When loading via `LoadEntityFromFile(filePath, startPos)`, the rigidbody's position is set to `startPos` after construction.



## Scene Config (`LoadSceneFromFile`)

**Used by:**
`LoadSceneFromFile(filePath)`

A scene config may contain:

* A **tile grid** (tileset + tilemap).
* An optional **song** config.
* A list of **entities** with positions.

### Top-Level Schema

Top-level keys:

* **`tileGrid`** — `object` (**required**)
* **`songPath`** — `string` (optional)
  Path to an **audio config JSON** (the same format used by `LoadAudioFromFile`, *not* a raw audio file).
* **`entities`** — `array<EntityPosPair>` (optional)

#### `tileGrid`

* **`tileSetPath`** — `string` (**required**)
  Path to the tileset image.
* **`tileSize`** — `integer` (**required**)
  Size of each tile in pixels (assumes square tiles).
* **`tileMapPath`** — `string` (**required**)
  Path to a tile map file (format expected by `RenderManager::LoadTileMap`).

If any of these are missing, the scene is considered invalid and error messages are logged.

#### `songPath` (optional)

* Path to an **audio config JSON** file, e.g.:

  ```json
  {
    "audioFilePath": "assets/audio/music/overworld_theme.ogg",
    "startVolume": 0.7,
    "isLooping": true
  }
  ```

The scene loader will call `LoadAudioFromFile(songPath)`.

#### `entities` (optional)

`entities` is an array of objects that map to `EntityPosPair`:

Each element:

* **`filePath`** — `string`
  Path to an **entity JSON** (as described in the Entity section).
* **`position`** — `array[number, number]`
  World position `[x, y]` for that entity.

If `position` has fewer than 2 elements, the position defaults to `(0, 0)`.

### Example Scene

```json
{
  "tileGrid": {
    "tileSetPath": "assets/tilesets/dungeon_tiles.png",
    "tileSize": 16,
    "tileMapPath": "assets/maps/dungeon_level_1.txt"
  },

  "songPath": "assets/audio/configs/dungeon_theme.json",

  "entities": [
    {
      "filePath": "assets/entities/player.json",
      "position": [2.0, 3.0]
    },
    {
      "filePath": "assets/entities/slime.json",
      "position": [5.5, 4.0]
    },
    {
      "filePath": "assets/entities/treasure_chest.json",
      "position": [10.0, 1.0]
    }
  ]
}
```

---

## Quick Reference

### Audio JSON

```json
{
  "audioFilePath": "string",
  "startVolume": 0.0,
  "isLooping": true
}
```

### Font JSON

```json
{
  "fontAtlasFilePath": "string",
  "fontAtlasOrder": "string",
  "charWidth": 8,
  "charHeight": 8,
  "horizontalPadding": 1
}
```

### Entity JSON

```json
{
  "scriptFilePath": "string",

  "rigidBody": {
    "mass": 1.0,
    "isStatic": false,

    "collisionCircle": {
      "radius": 0.5,
      "offset": [0.0, 0.0],
      "isTrigger": false,
      "collisionMask": 0
    }

    // OR

    "collisionRectangle": {
      "width": 1.0,
      "height": 1.0,
      "offset": [0.0, 0.0],
      "isTrigger": false,
      "collisionMask": 0
    }
  },

  "sprite": {
    "spriteFilePath": "string",
    "scale": [1.0, 1.0]
  }

  // OR

  "spriteSheet": {
    "spriteFilePath": "string",
    "spriteWidth": 16,
    "spriteHeight": 16,
    "scale": [1.0, 1.0],
    "startSpriteIndex": [0, 0]
  }
}
```

### Scene JSON

```json
{
  "tileGrid": {
    "tileSetPath": "string",
    "tileSize": 16,
    "tileMapPath": "string"
  },

  "songPath": "path/to/audio_config.json",

  "entities": [
    {
      "filePath": "path/to/entity.json",
      "position": [0, 0]
    }
  ]
}
```
