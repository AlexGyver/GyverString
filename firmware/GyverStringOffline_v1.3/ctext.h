enum colors {
	C_BLACK  = 0x000000,
	C_GREEN  = 0x00FF00,
	C_RED    = 0xFF0000,
	C_BLUE   = 0x0000FF,
	C_PURPLE = 0xFF00FF,
	C_YELLOW = 0xFFFF00,
	C_CYAN   = 0x00FFFF,
	C_WHITE  = 0xFFFFFF,
};

enum animations {
	RIGHT_TO_LEFT,
	LEFT_TO_RIGHT,
	UP_TO_DOWN,
	DOWN_TO_UP,
};

enum rainbowfades {
  RB_CONSTANT,
  RB_FADE_IN, // fade in on sticking time
  RB_FADE_OUT // fade out on sticking time
};

typedef struct ctext {
	uint32_t color;
	uint8_t anim;
	uint16_t sticktime; // замирание текста на * миллисек после столкновения с границей экрана
	uint8_t rainbow; // прозрачность радужного эффекта
	uint8_t rainbowfade;
	String text;
} ctext;
