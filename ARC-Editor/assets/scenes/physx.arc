Scene: Untitled
Entities:
  - Entity: 10635279447234847427
    Components:
      - Name: CNameComponent
        Data:
          Name: Atom3
      - Name: CTransform2DComponent
        Data:
          Location: [1.39999998, 3.0999999, 0]
          Rotation: 0
          Scale: [1, 1]
      - Name: CMassComponent
        Data:
          Mass: 1
      - Name: CRigidBody2DComponent
        Data:
          Type: 2
          bFixedRotation: false
      - Name: CCircleCollider2DComponent
        Data:
          Offset: [0, 0]
          Radius: 0.5
          Density: 1
          Friction: 0.5
          Restitution: 0
          RestitutionThreshhold: 0.5
      - Name: CCircleRendererComponent
        Data:
          Color: [1, 1, 1, 1]
          Thickness: 1
          Sharpness: 0.995000005
  - Entity: 7977062694123731637
    Components:
      - Name: CNameComponent
        Data:
          Name: Atom2
      - Name: CTransform2DComponent
        Data:
          Location: [-4.0999999, 0, 0]
          Rotation: 5.56934595
          Scale: [6.19999981, 1]
      - Name: CSpriteRendererComponent
        Data:
          Color: [0, 0, 1, 1]
          TexturePath: C:\Users\Arnav\Documents\[ARC]\CPP\ARC-Engine\ARC-Editor\assets\textures\Checkerboard.png
          TextureScaling: [1, 1]
      - Name: CMassComponent
        Data:
          Mass: 1
      - Name: CRigidBody2DComponent
        Data:
          Type: 0
          bFixedRotation: false
      - Name: CBoxCollider2DComponent
        Data:
          Offset: [0, 0]
          Size: [0.5, 0.5]
          Density: 1
          Friction: 0.5
          Restitution: 0
          RestitutionThreshhold: 0.5
  - Entity: 15619856859172032182
    Components:
      - Name: CNameComponent
        Data:
          Name: Atom1
      - Name: CTransform2DComponent
        Data:
          Location: [2.20000005, 0, 0]
          Rotation: 0.691150367
          Scale: [6.80000019, 1]
      - Name: CSpriteRendererComponent
        Data:
          Color: [1, 1, 1, 1]
      - Name: CMassComponent
        Data:
          Mass: 1
      - Name: CRigidBody2DComponent
        Data:
          Type: 0
          bFixedRotation: false
      - Name: CBoxCollider2DComponent
        Data:
          Offset: [0, 0]
          Size: [0.5, 0.5]
          Density: 1
          Friction: 0.5
          Restitution: 0
          RestitutionThreshhold: 0.5
  - Entity: 9646306359375905791
    Components:
      - Name: CNameComponent
        Data:
          Name: Camera
      - Name: CTransform2DComponent
        Data:
          Location: [0, 0, 0]
          Rotation: 0
          Scale: [1, 1]
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