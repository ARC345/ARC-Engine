Scene: Untitled
Entities:
  - Entity: 11977879731639005334
    Components:
      - Name: CNameComponent
        Data:
          Name: Atom3
      - Name: CSpriteRendererComponent
        Data:
          Color: [0, 1, 0, 1]
          TexturePath: assets/textures/circle-64.png
          TextureScaling: [1, 1]
      - Name: CTransform2DComponent
        Data:
          Location: [0, -10, 0]
          Rotation: 0
          Scale: [1, 1]
      - Name: CMassComponent
        Data:
          Mass: 1
  - Entity: 12962362642731797207
    Components:
      - Name: CNameComponent
        Data:
          Name: Empty Entity
  - Entity: 6572504786699224861
    Components:
      - Name: CNameComponent
        Data:
          Name: Atom1
      - Name: CSpriteRendererComponent
        Data:
          Color: [1, 1, 0, 1]
          TexturePath: assets/textures/Rocksquare.png
          TextureScaling: [1, 1]
      - Name: CTransform2DComponent
        Data:
          Location: [-10, 0, 0]
          Rotation: 0
          Scale: [1, 1]
      - Name: CMassComponent
        Data:
          Mass: 1
  - Entity: 7696908021652172977
    Components:
      - Name: CCameraComponent
        Data:
          Primary: true
          FixedAspectRatio: false
          ProjectionType: 1
          Perspective:
            FOV: 0.785398185
            NearClip: 0.00999999978
            FarClip: 1
          Orthographic:
            Size: 16
            NearClip: -1
            FarClip: 1
      - Name: CNameComponent
        Data:
          Name: Camera
      - Name: CTransform2DComponent
        Data:
          Location: [0, 0, 0]
          Rotation: 0
          Scale: [1, 1]