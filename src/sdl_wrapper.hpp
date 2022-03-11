#include "mcinterface.h"

const static int WINDOW_WIDTH = 856;
const static int WINDOW_HEIGHT = 480;

enum class KeyEvent {
    VK_W,
    VK_A,
    VK_S,
    VK_D,
    VK_SPACE,
    VK_PERIOD,
    VK_COMMA,
    VK_Q,
    MOUSE_LEFT,
    MOUSE_RIGHT,
};

class KeySet {
    int states[10];

public:
    void erase(KeyEvent k) {
        states[static_cast<int>(k)] = 0;
    }

    void insert(KeyEvent k) {
        states[static_cast<int>(k)] = 1;
    }

    void set(KeyEvent k, bool state) {
	states[static_cast<int>(k)] = state;
    }

    bool contains(KeyEvent k) {
        return states[static_cast<int>(k)] != 0;
    }
};

static KeySet input {};

const int SCR_RES_X = (int) (107);
const int SCR_RES_Y = (int) (60);

struct Color {
    unsigned char r, g, b, a;

    Color(int r2, int g2, int b2, int a2): r(r2), g(g2), b(b2), a(a2) {}

    Color darker() {
        // TODO:
        return *this;
    }

    static Color red() {
        return Color(255, 0, 0, 255);
    }

    static Color white() {
        return Color(255, 255, 255, 255);
    }

    static Color gray() {
        return Color(127, 127, 127, 255);
    }
};

class BasicStroke {
public:
    BasicStroke(int w) : width(w) {}

    int width;
};

struct ColorRGBA {
	int r, g, b, a;

	int toInt() {
		return (a << 24) | (r << 16) | (g << 8) | (b << 0);
	}
};

ColorRGBA rgba(int r, int g, int b, int a) {
	return { r, g, b, a };
}

ColorRGBA colorFromBlock(Block b) {
	switch (b) {
		case AIR: return rgba(0, 0, 0, 0);
		case COBBLESTONE: return rgba(127, 127, 127, 0xFF);
		case GRANITE: return rgba(255, 127, 0, 0xFF);
		case ANDESITE: return rgba(170, 170, 170, 0xFF);
		case DIORITE: return rgba(200, 200, 200, 0xFF);
		case LAPIS_BLOCK: return rgba(0, 0, 255, 0xFF);
		case IRON_BLOCK: return rgba(255, 255, 255, 0xFF);
		case GOLD_BLOCK: return rgba(255, 255, 0, 0xFF);
		case DIAMOND_BLOCK: return rgba(0, 127, 255, 0xFF);
		case REDSTONE_BLOCK: return rgba(255, 0, 0, 0xFF);
		case EMERALD_BLOCK: return rgba(0, 255, 0, 0xFF);
		case DIRT_BLOCK: return rgba(0x6C, 0x4E, 0x36, 0xFF);
		case OAK_LOG_BLOCK: return rgba(0x6C, 0x56, 0x34, 0xFF);
		case OAK_LEAVES_BLOCK: return rgba(0x21, 0x53, 0x0C, 0xFF);
	}
}

class BufferedImage2 {
public:
	Block data[16 * 16 * 3 * 16];
	int w;
	int h;

    BufferedImage2(int w, int h, int ty) : w(w), h(h) {
    }

    Block *getDataBuffer() {
        // TODO:
        return data;
    }

    size_t getDataBufferLen() {
        // TODO:
        return w * h;
    }

    void setRGB(int x, int y, int color) {
        // TODO:
    }

    static const int TYPE_INT_ARGB = 2;
};


#ifdef HOSTED

class BufferedImage {
public:
	Block *data;
	int w;
	int h;

    BufferedImage(int w, int h, int ty) : w(w), h(h) {
        // TODO:
        data = new Block[w * h];
    }

    Block *getDataBuffer() {
        // TODO:
        return data;
    }

    size_t getDataBufferLen() {
        // TODO:
        return w * h;
    }

    void setRGB(int x, int y, int color) {
        // TODO:
    }

    static const int TYPE_INT_ARGB = 2;
};



BufferedImage screen = BufferedImage(SCR_RES_X, SCR_RES_Y, 1);

void setPixel(int x, int y, Block pixel) {
	screen.getDataBuffer()[x + y * screen.w] = pixel;
}

#include <SDL2/SDL.h>
//#include <SDL2/SDL_ttf.h>
#include <cassert>
#include <iostream>
#include <math.h>

void print(int i) {
	std::cout << "Printed " << i << "\n";
}

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

void sdl2_init() {
	assert(SDL_Init(SDL_INIT_VIDEO) >= 0);

	//assert(TTF_Init() == 0);

	window = SDL_CreateWindow("Minecraft4k", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	assert(window != NULL);

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

void setupInput() {}

void handleEvents() {
	SDL_Event evt;
	while (SDL_PollEvent(&evt) != 0) {
		if (evt.type == SDL_QUIT) {
			printf("Thank you for playing wing commander\n");
			assert(false);
		} else if (evt.type == SDL_KEYDOWN) {
			switch (evt.key.keysym.sym) {
				case SDLK_w: input.insert(KeyEvent::VK_W); break;
				case SDLK_a: input.insert(KeyEvent::VK_A); break;
				case SDLK_s: input.insert(KeyEvent::VK_S); break;
				case SDLK_d: input.insert(KeyEvent::VK_D); break;
				case SDLK_q: input.insert(KeyEvent::VK_Q); break;
				case SDLK_SPACE: input.insert(KeyEvent::VK_SPACE); break;
				case SDLK_COMMA: input.insert(KeyEvent::VK_COMMA); break;
				case SDLK_PERIOD: input.insert(KeyEvent::VK_PERIOD); break;
				case SDLK_ESCAPE: {
					printf("Thank you for playing wing commander\n");
					assert(false);
				};
				default: break;
			}
		} else if (evt.type == SDL_KEYUP) {
			switch (evt.key.keysym.sym) {
				case SDLK_w: input.erase(KeyEvent::VK_W); break;
				case SDLK_a: input.erase(KeyEvent::VK_A); break;
				case SDLK_s: input.erase(KeyEvent::VK_S); break;
				case SDLK_d: input.erase(KeyEvent::VK_D); break;
				case SDLK_q: input.erase(KeyEvent::VK_Q); break;
				case SDLK_SPACE: input.erase(KeyEvent::VK_SPACE); break;
				case SDLK_COMMA: input.erase(KeyEvent::VK_COMMA); break;
				case SDLK_PERIOD: input.erase(KeyEvent::VK_PERIOD); break;
				default: break;
			}
		} else if (evt.type == SDL_MOUSEBUTTONDOWN) {
			switch (evt.button.button) {
				case SDL_BUTTON_LEFT: input.insert(KeyEvent::MOUSE_LEFT); break;
				case SDL_BUTTON_RIGHT: input.insert(KeyEvent::MOUSE_RIGHT); break;
				default: break;
			}
		} else if (evt.type == SDL_MOUSEBUTTONUP) {
			switch (evt.button.button) {
				case SDL_BUTTON_LEFT: input.erase(KeyEvent::MOUSE_LEFT); break;
				case SDL_BUTTON_RIGHT: input.erase(KeyEvent::MOUSE_RIGHT); break;
				default: break;
			}
		}
	}
}

class JFrame {
public:
	JFrame() {
		sdl2_init();
        // TODO:
		/*
		addEventListender(MinecraftEventListener());

		setSize(WINDOW_WIDTH, WINDOW_HEIGHT);
		setResizable(false);
		setDefaultCloseOperation(JFrame::EXIT_ON_CLOSE);
		setLocationRelativeTo(null); // center the window

		// add Minecraft!
		getContentPane().add(m4k);

		setVisible(true);*/
	}
};

class Graphics2D {

public:
	Graphics2D() {
	}

	void clear() {
		drawImage(screen, 0, 0);
	}

	void drawImage(BufferedImage &img, int x, int y) {
		drawImage(img, x, y, img.w, img.h);
	}

	void drawImage(BufferedImage &img, int x, int y, int w, int h) {
		drawImage(img, x, y, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0, w, h);
	}

	void drawImage(BufferedImage &img, int dstx0, int dsty0, int dstx1, int dsty1, int srcx0, int srcy0, int srcx1, int srcy1) {
		SDL_Surface *s = SDL_CreateRGBSurface(0, img.w, img.h, 32, (0xFF << 16), (0xFF << 8), (0xFF << 0), (0xFF << 24));
		assert(s != NULL);

		SDL_LockSurface(s);
		int *pixels = (int*)s->pixels;
		for (int i = 0; i < s->h * s->w; ++i) {
			pixels[i] = colorFromBlock(img.data[i]).toInt();
		}
		SDL_UnlockSurface(s);

		SDL_Texture *t = SDL_CreateTextureFromSurface(renderer, s);
		assert(t != NULL);

		SDL_Rect src = { srcx0, srcy0, srcx1 - srcx0, srcy1 - srcy0 };
		SDL_Rect dst = { dstx0, dsty0, dstx1 - dstx0, dsty1 - dsty0 };
		assert(SDL_RenderCopy(renderer, t, &src, &dst) == 0);

		SDL_DestroyTexture(t);
		SDL_FreeSurface(s);
	}

	void setColor(Color color) {
		SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
		std::cout << "set color! a: " << int(color.r) << " " << int(color.g) << " " << int(color.b) << "\n";
	}

	void setStroke(BasicStroke stroke) {
		// TODO:
	}

	void fillRect(int x, int y, int w, int h) {
		SDL_Rect rect { x, y, w, h };
		SDL_RenderFillRect(renderer, &rect);
	}

	void drawRect(int x, int y, int w, int h) {
		SDL_Rect rect { x, y, w, h };
		SDL_RenderDrawRect(renderer, &rect);

		std::cout << "drew a rect! " << x << " " << y << " " << w << " " << h << "\n";
	}

	/*void setFont(Font font) {
		// TODO:
	}

	void drawString(std::string &s, int x, int y) {
		SDL_Surface *msg = TTF_RenderText_Solid(sans, s.c_str(), SDL_Color { 255, 255, 255, 255 });

		SDL_Texture *t = SDL_CreateTextureFromSurface(renderer, msg);
		assert(t != NULL);

		SDL_Rect dst = { x, y, msg->w, msg->h };
		assert(SDL_RenderCopy(renderer, t, NULL, &dst) == 0);

		SDL_DestroyTexture(t);
		SDL_FreeSurface(msg);
	}*/
};

void repaint() {
	SDL_RenderPresent(renderer);
}

Graphics2D g2d {};

#else

#define assert(X) ((void)(X))

struct Graphics2D {
	void clear() {
		// Do nothing...
	}
};

Graphics2D g2d {};

void setupInput() {
	turtle_y(27);

	turtle_x(52); turtle_z(-74);
	turtle_set(Block::DIORITE);

	turtle_x(52); turtle_z(-76);
	turtle_set(Block::DIORITE);

	turtle_x(53); turtle_z(-75);
	turtle_set(Block::DIORITE);

	turtle_x(51); turtle_z(-75);
	turtle_set(Block::DIORITE);

	turtle_x(54); turtle_z(-74);
	turtle_set(Block::ANDESITE);

	turtle_x(50);
	turtle_set(Block::ANDESITE);
}

void handleEvents() {
	turtle_y(28);

	turtle_x(52); turtle_z(-74);
	input.set(KeyEvent::VK_W, turtle_get() == Block::COBBLESTONE);

	turtle_x(52); turtle_z(-76);
	input.set(KeyEvent::VK_S, turtle_get() == Block::COBBLESTONE);

	turtle_x(53); turtle_z(-75);
	input.set(/*KeyEvent::VK_A*/ KeyEvent::VK_COMMA, turtle_get() == Block::COBBLESTONE);

	turtle_x(51); turtle_z(-75);
	input.set(/*KeyEvent::VK_D*/ KeyEvent::VK_PERIOD, turtle_get() == Block::COBBLESTONE);

	turtle_x(54); turtle_z(-74);
	input.set(KeyEvent::MOUSE_LEFT, turtle_get() == Block::COBBLESTONE);

	turtle_x(50);
	input.set(KeyEvent::MOUSE_RIGHT, turtle_get() == Block::COBBLESTONE);
}

void repaint() {
	turtle_x(0); turtle_y(1); turtle_z(-18);
	turtle_set(Block::REDSTONE_BLOCK);
	turtle_set(Block::AIR);
	// Do nothing...
}

class JFrame {};

void setPixel(int x, int y, Block color) {
	turtle_z(-20);
	turtle_x(x);
	turtle_y(SCR_RES_Y - 1 - y);
	turtle_set(color);
}

// Count is measured in INTEGERS, not BYTES!!!
void memset_aligned(int *dst, int value, size_t count) {
	for (size_t i = 0; i < count; ++i) {
		dst[i] = value;
		if (i % 1024 == 0) {
			print(i / 1024);
			sleep();
		}
	}
	print(4321);
}

// From http://git.musl-libc.org/cgit/musl/tree/src/string/memset.c
extern "C" void *memset(void *dest, int c, size_t n)
{
	if ((size_t)dest % 4 == 0 && n % 4 == 0) {
		int c2 = (c << 8) | c;
		c2 |= (c2 << 16);
		memset_aligned((int*)dest, c, n / 4);
		return dest;
	}

	unsigned char *s = (unsigned char*)dest;
	size_t k;

	/* Fill head and tail with minimal branching. Each
	 * conditional ensures that all the subsequently used
	 * offsets are well-defined and in the dest region. */

	if (!n) return dest;
	s[0] = c;
	s[n-1] = c;
	if (n <= 2) return dest;
	s[1] = c;
	s[2] = c;
	s[n-2] = c;
	s[n-3] = c;
	if (n <= 6) return dest;
	s[3] = c;
	s[n-4] = c;
	if (n <= 8) return dest;

	/* Advance pointer to align it at a 4-byte boundary,
	 * and truncate n to a multiple of 4. The previous code
	 * already took care of any head/tail that get cut off
	 * by the alignment. */

	k = -(uintptr_t)s & 3;
	s += k;
	n -= k;
	n &= -4;

  /* Pure C fallback with no aliasing violations. */
	for (; n; n--, s++) *s = c;

  return dest;
}


#endif