#pragma once

/* The unknown key */
#define ARC_KEY_UNKNOWN            -1

/* Printable keys */
#define ARC_KEY_SPACE              32
#define ARC_KEY_APOSTROPHE         39  /* ' */
#define ARC_KEY_COMMA              44  /* , */
#define ARC_KEY_MINUS              45  /* - */
#define ARC_KEY_PERIOD             46  /* . */
#define ARC_KEY_SLASH              47  /* / */
#define ARC_KEY_0                  48
#define ARC_KEY_1                  49
#define ARC_KEY_2                  50
#define ARC_KEY_3                  51
#define ARC_KEY_4                  52
#define ARC_KEY_5                  53
#define ARC_KEY_6                  54
#define ARC_KEY_7                  55
#define ARC_KEY_8                  56
#define ARC_KEY_9                  57
#define ARC_KEY_SEMICOLON          59  /* ; */
#define ARC_KEY_EQUAL              61  /* = */
#define ARC_KEY_A                  65
#define ARC_KEY_B                  66
#define ARC_KEY_C                  67
#define ARC_KEY_D                  68
#define ARC_KEY_E                  69
#define ARC_KEY_F                  70
#define ARC_KEY_G                  71
#define ARC_KEY_H                  72
#define ARC_KEY_I                  73
#define ARC_KEY_J                  74
#define ARC_KEY_K                  75
#define ARC_KEY_L                  76
#define ARC_KEY_M                  77
#define ARC_KEY_N                  78
#define ARC_KEY_O                  79
#define ARC_KEY_P                  80
#define ARC_KEY_Q                  81
#define ARC_KEY_R                  82
#define ARC_KEY_S                  83
#define ARC_KEY_T                  84
#define ARC_KEY_U                  85
#define ARC_KEY_V                  86
#define ARC_KEY_W                  87
#define ARC_KEY_X                  88
#define ARC_KEY_Y                  89
#define ARC_KEY_Z                  90
#define ARC_KEY_LEFT_BRACKET       91  /* [ */
#define ARC_KEY_BACKSLASH          92  /* \ */
#define ARC_KEY_RIGHT_BRACKET      93  /* ] */
#define ARC_KEY_GRAVE_ACCENT       96  /* ` */
#define ARC_KEY_WORLD_1            161 /* non-US #1 */
#define ARC_KEY_WORLD_2            162 /* non-US #2 */

/* Function keys */
#define ARC_KEY_ESCAPE             256
#define ARC_KEY_ENTER              257
#define ARC_KEY_TAB                258
#define ARC_KEY_BACKSPACE          259
#define ARC_KEY_INSERT             260
#define ARC_KEY_DELETE             261
#define ARC_KEY_RIGHT              262
#define ARC_KEY_LEFT               263
#define ARC_KEY_DOWN               264
#define ARC_KEY_UP                 265
#define ARC_KEY_PAGE_UP            266
#define ARC_KEY_PAGE_DOWN          267
#define ARC_KEY_HOME               268
#define ARC_KEY_END                269
#define ARC_KEY_CAPS_LOCK          280
#define ARC_KEY_SCROLL_LOCK        281
#define ARC_KEY_NUM_LOCK           282
#define ARC_KEY_PRINT_SCREEN       283
#define ARC_KEY_PAUSE              284
#define ARC_KEY_F1                 290
#define ARC_KEY_F2                 291
#define ARC_KEY_F3                 292
#define ARC_KEY_F4                 293
#define ARC_KEY_F5                 294
#define ARC_KEY_F6                 295
#define ARC_KEY_F7                 296
#define ARC_KEY_F8                 297
#define ARC_KEY_F9                 298
#define ARC_KEY_F10                299
#define ARC_KEY_F11                300
#define ARC_KEY_F12                301
#define ARC_KEY_F13                302
#define ARC_KEY_F14                303
#define ARC_KEY_F15                304
#define ARC_KEY_F16                305
#define ARC_KEY_F17                306
#define ARC_KEY_F18                307
#define ARC_KEY_F19                308
#define ARC_KEY_F20                309
#define ARC_KEY_F21                310
#define ARC_KEY_F22                311
#define ARC_KEY_F23                312
#define ARC_KEY_F24                313
#define ARC_KEY_F25                314
#define ARC_KEY_KP_0               320
#define ARC_KEY_KP_1               321
#define ARC_KEY_KP_2               322
#define ARC_KEY_KP_3               323
#define ARC_KEY_KP_4               324
#define ARC_KEY_KP_5               325
#define ARC_KEY_KP_6               326
#define ARC_KEY_KP_7               327
#define ARC_KEY_KP_8               328
#define ARC_KEY_KP_9               329
#define ARC_KEY_KP_DECIMAL         330
#define ARC_KEY_KP_DIVIDE          331
#define ARC_KEY_KP_MULTIPLY        332
#define ARC_KEY_KP_SUBTRACT        333
#define ARC_KEY_KP_ADD             334
#define ARC_KEY_KP_ENTER           335
#define ARC_KEY_KP_EQUAL           336
#define ARC_KEY_LEFT_SHIFT         340
#define ARC_KEY_LEFT_CONTROL       341
#define ARC_KEY_LEFT_ALT           342
#define ARC_KEY_LEFT_SUPER         343
#define ARC_KEY_RIGHT_SHIFT        344
#define ARC_KEY_RIGHT_CONTROL      345
#define ARC_KEY_RIGHT_ALT          346
#define ARC_KEY_RIGHT_SUPER        347
#define ARC_KEY_MENU               348

#define ARC_KEY_LAST               ARC_KEY_MENU

/*! @} */

/*! @defgroup mods Modifier key flags
 *  @brief Modifier key flags.
 *
 *  See [key input](@ref input_key) for how these are used.
 *
 *  @ingroup input
 *  @{ */

 /*! @brief If this bit is set one or more Shift keys were held down.
  *
  *  If this bit is set one or more Shift keys were held down.
  */
#define ARC_MOD_SHIFT           0x0001
  /*! @brief If this bit is set one or more Control keys were held down.
   *
   *  If this bit is set one or more Control keys were held down.
   */
#define ARC_MOD_CONTROL         0x0002
   /*! @brief If this bit is set one or more Alt keys were held down.
	*
	*  If this bit is set one or more Alt keys were held down.
	*/
#define ARC_MOD_ALT             0x0004
	/*! @brief If this bit is set one or more Super keys were held down.
	 *
	 *  If this bit is set one or more Super keys were held down.
	 */
#define ARC_MOD_SUPER           0x0008
	 /*! @brief If this bit is set the Caps Lock key is enabled.
	  *
	  *  If this bit is set the Caps Lock key is enabled and the @ref
	  *  ARC_LOCK_KEY_MODS input mode is set.
	  */
#define ARC_MOD_CAPS_LOCK       0x0010
	  /*! @brief If this bit is set the Num Lock key is enabled.
	   *
	   *  If this bit is set the Num Lock key is enabled and the @ref
	   *  ARC_LOCK_KEY_MODS input mode is set.
	   */
#define ARC_MOD_NUM_LOCK        0x0020

	   /*! @} */

	   /*! @defgroup buttons Mouse buttons
		*  @brief Mouse button IDs.
		*
		*  See [mouse button input](@ref input_mouse_button) for how these are used.
		*
		*  @ingroup input
		*  @{ */
#define ARC_MOUSE_BUTTON_1         0
#define ARC_MOUSE_BUTTON_2         1
#define ARC_MOUSE_BUTTON_3         2
#define ARC_MOUSE_BUTTON_4         3
#define ARC_MOUSE_BUTTON_5         4
#define ARC_MOUSE_BUTTON_6         5
#define ARC_MOUSE_BUTTON_7         6
#define ARC_MOUSE_BUTTON_8         7
#define ARC_MOUSE_BUTTON_LAST      ARC_MOUSE_BUTTON_8
#define ARC_MOUSE_BUTTON_LEFT      ARC_MOUSE_BUTTON_1
#define ARC_MOUSE_BUTTON_RIGHT     ARC_MOUSE_BUTTON_2
#define ARC_MOUSE_BUTTON_MIDDLE    ARC_MOUSE_BUTTON_3
		/*! @} */

		/*! @defgroup joysticks Joysticks
		 *  @brief Joystick IDs.
		 *
		 *  See [joystick input](@ref joystick) for how these are used.
		 *
		 *  @ingroup input
		 *  @{ */
#define ARC_JOYSTICK_1             0
#define ARC_JOYSTICK_2             1
#define ARC_JOYSTICK_3             2
#define ARC_JOYSTICK_4             3
#define ARC_JOYSTICK_5             4
#define ARC_JOYSTICK_6             5
#define ARC_JOYSTICK_7             6
#define ARC_JOYSTICK_8             7
#define ARC_JOYSTICK_9             8
#define ARC_JOYSTICK_10            9
#define ARC_JOYSTICK_11            10
#define ARC_JOYSTICK_12            11
#define ARC_JOYSTICK_13            12
#define ARC_JOYSTICK_14            13
#define ARC_JOYSTICK_15            14
#define ARC_JOYSTICK_16            15
#define ARC_JOYSTICK_LAST          ARC_JOYSTICK_16
		 /*! @} */

		 /*! @defgroup gamepad_buttons Gamepad buttons
		  *  @brief Gamepad buttons.
		  *
		  *  See @ref gamepad for how these are used.
		  *
		  *  @ingroup input
		  *  @{ */
#define ARC_GAMEPAD_BUTTON_A               0
#define ARC_GAMEPAD_BUTTON_B               1
#define ARC_GAMEPAD_BUTTON_X               2
#define ARC_GAMEPAD_BUTTON_Y               3
#define ARC_GAMEPAD_BUTTON_LEFT_BUMPER     4
#define ARC_GAMEPAD_BUTTON_RIGHT_BUMPER    5
#define ARC_GAMEPAD_BUTTON_BACK            6
#define ARC_GAMEPAD_BUTTON_START           7
#define ARC_GAMEPAD_BUTTON_GUIDE           8
#define ARC_GAMEPAD_BUTTON_LEFT_THUMB      9
#define ARC_GAMEPAD_BUTTON_RIGHT_THUMB     10
#define ARC_GAMEPAD_BUTTON_DPAD_UP         11
#define ARC_GAMEPAD_BUTTON_DPAD_RIGHT      12
#define ARC_GAMEPAD_BUTTON_DPAD_DOWN       13
#define ARC_GAMEPAD_BUTTON_DPAD_LEFT       14
#define ARC_GAMEPAD_BUTTON_LAST            ARC_GAMEPAD_BUTTON_DPAD_LEFT

#define ARC_GAMEPAD_BUTTON_CROSS       ARC_GAMEPAD_BUTTON_A
#define ARC_GAMEPAD_BUTTON_CIRCLE      ARC_GAMEPAD_BUTTON_B
#define ARC_GAMEPAD_BUTTON_SQUARE      ARC_GAMEPAD_BUTTON_X
#define ARC_GAMEPAD_BUTTON_TRIANGLE    ARC_GAMEPAD_BUTTON_Y
		  /*! @} */

		  /*! @defgroup gamepad_axes Gamepad axes
		   *  @brief Gamepad axes.
		   *
		   *  See @ref gamepad for how these are used.
		   *
		   *  @ingroup input
		   *  @{ */
#define ARC_GAMEPAD_AXIS_LEFT_X        0
#define ARC_GAMEPAD_AXIS_LEFT_Y        1
#define ARC_GAMEPAD_AXIS_RIGHT_X       2
#define ARC_GAMEPAD_AXIS_RIGHT_Y       3
#define ARC_GAMEPAD_AXIS_LEFT_TRIGGER  4
#define ARC_GAMEPAD_AXIS_RIGHT_TRIGGER 5
#define ARC_GAMEPAD_AXIS_LAST          ARC_GAMEPAD_AXIS_RIGHT_TRIGGER
		   /*! @} */
