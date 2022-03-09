/*
package minecraft4k;

import java.awt.BasicStroke;
import java.awt.Color;
import java.awt.Font;
import java.awt.Graphics2D;
import java.awt.HeadlessException;
import java.awt.MouseInfo;
import java.awt.Point;
import java.awt.event.FocusEvent;
import java.awt.event.KeyEvent;
import java.awt.event.MouseEvent;
import java.awt.event.MouseWheelEvent;
import java.awt.image.BufferedImage;
import java.awt.image.DataBufferInt;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.Random;
import javax.swing.JFrame;
import javax.swing.JPanel;
*/

#include <limits.h>
#include <stddef.h>
#include <stdint.h>

#include "sdl_wrapper.hpp"
#include "fix16.hpp"

class Vec3
{
public:
    Fix16 x, y, z;
    
    Vec3(Fix16 _x, Fix16 _y, Fix16 _z)
    {
        x = _x;
        y = _y;
        z = _z;
    }
    
    Vec3(Fix16 val)
    {
        x = val;
        y = val;
        z = val;
    }
    
    Vec3() : x(0.0), y(0.0), z(0.0)
    {
    }
    
    bool isZero()
    {
        return this->x == 0 && this->y == 0 && this->z == 0;
    }
    
    static void add(Vec3 a, Vec3 b, Vec3 &out)
    {
        out.x = a.x + b.x;
        out.y = a.y + b.y;
        out.z = a.z + b.z;
    }
    
    static void sub(Vec3 a, Vec3 b, Vec3 &out)
    {
        out.x = a.x - b.x;
        out.y = a.y - b.y;
        out.z = a.z - b.z;
    }
    
    static Fix16 dot(Vec3 a, Vec3 b)
    {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }
    
    static void mult(Vec3 a, Vec3 b, Vec3 &out)
    {
        out.x = a.x * b.x;
        out.y = a.y * b.y;
        out.z = a.z * b.z;
    }
    
    static void mult(Vec3 a, Fix16 b, Vec3 &out)
    {
        out.x = a.x * b;
        out.y = a.y * b;
        out.z = a.z * b;
    }
    
    static void normalize(Vec3 vec, Vec3 &out)
    {
       Fix16 length = Vec3::length(vec);

       out.x = vec.x / length;
       out.y = vec.y / length;
       out.z = vec.z / length;
    }

    static Vec3 normalize_small(Vec3 vec) {
        Fix16 length = Vec3::length_small(vec);

        Fix16 x = (((int32_t)vec.x << 10) / (int32_t)length) << 6;
        Fix16 y = (((int32_t)vec.y << 10) / (int32_t)length) << 6;
        Fix16 z = (((int32_t)vec.z << 10) / (int32_t)length) << 6;

        return Vec3(x, y, z);
    }
    
    static Fix16 length(Vec3 vec)
    {
        return (vec.x * vec.x + vec.y * vec.y + vec.z * vec.z).sqrt();
    }
    
    static Fix16 length_small(Vec3 vec)
    {
        int32_t x = (int32_t(vec.x) >> 6);
        int32_t y = (int32_t(vec.y) >> 6);
        int32_t z = (int32_t(vec.z) >> 6);
        return Fix16((x * x + y * y + z * z) >> 4).sqrt();
    }
    
    static void lerp(Vec3 start, Vec3 end, Fix16 t, Vec3 &out)
    {
        out.x = start.x + (end.x - start.x) * t;
        out.y = start.y + (end.y - start.y) * t;
        out.z = start.z + (end.z - start.z) * t;
    }
    
    static Vec3 fromRGB(int r, int g, int b)
    {
        return Vec3(Fix16((int16_t)r) / 255.0, Fix16((int16_t)g) / 255.0, Fix16((int16_t)b) / 255.0);
    }

    static Vec3 fromRGB(Block block) {
        ColorRGBA c = colorFromBlock(block);
        return Vec3::fromRGB(c.r, c.g, c.b);
    }
    
    static Vec3 fromRGB(int rgb)
    {
        return  fromRGB(rgb >> 16 & 0xFF,
                        rgb >> 8 & 0xFF,
                        rgb & 0xFF);
    }
};

class Random {
    long seed;
public:
	Random(long seed) {
        setSeed(seed);
	}

    void setSeed(long s) {
        seed = s;
    }

	Fix16 nextFloat() {
        return Fix16((fix16_t)(nextInt() & 0xFFFF));
	}

    int nextInt() {
        // Numbers from https://www.johndcook.com/blog/2017/07/05/simple-random-number-generator/
        const unsigned a = 742938285;   // multiplier
        const unsigned e = 31;          // will need this later
        const unsigned m = (1u<<e) - 1; // modulus
 
        seed = (long)((a * (unsigned)seed) % m);
        assert((int)seed > 0);
        return (int)seed;    
    }

	int nextInt(int limit) {
        assert(limit > 0);

        return nextInt() % limit;
	}
};

struct Point {
    int x; int y;

    Point() : x(0), y(0) {}

    Point(int x1, int y1) : x(x1), y(y1) {}
};


class MouseWheelEvent {
public:
    int getUnitsToScroll() {
        // TODO:
        return 0;
    }
};

long currentTimeMillis() {
    static long time = 0;
    time += 10;
	return time;
}

Point getMouseLocation() {
    static int x = 0;

    // TODO:
    return { x, x };
}

void sleep_ms(int ms) {
    // TODO:
}

int imax(int a, int b) {
    if (a > b) {
        return a;
    } else {
        return b;
    }
}

/* ================ */

const bool classic = false;
const bool classicTerrain = false;
const bool classicTextures = false;
const bool classicRender = true;

const Fix16 RENDER_DIST = Fix16(20.0);
const Fix16 PLAYER_REACH = Fix16(5.0f);

const int TEXTURE_RES = 16;

const int WORLD_SIZE = 32;
const int WORLD_HEIGHT = 64;

enum class Axis {
    X,
    Y,
    Z,
};

enum class BlockId {
	BLOCK_AIR = 0,
	BLOCK_GRASS = 1,
	BLOCK_DEFAULT_DIRT = 2,
	BLOCK_STONE = 4,
	BLOCK_BRICKS = 5,
	BLOCK_WOOD = 7,
	BLOCK_LEAVES = 8
};

const int PERLIN_RES = 1024;

JFrame frame;

Point mouseDelta = { 0, 0 };
long lastMouseMove = currentTimeMillis();

/*const static int CROSS_SIZE = 32;

static BufferedImage crosshair = BufferedImage(CROSS_SIZE, CROSS_SIZE, BufferedImage::TYPE_INT_ARGB);*/

// COLORS
const Vec3 FOG_COLOR = Vec3(1);

// S = Sun, A = Amb, Y = skY
const Vec3 SC_DAY = Vec3(1);
const Vec3 AC_DAY = Vec3(0.5f, 0.5f, 0.5f);
const Vec3 YC_DAY = Vec3::fromRGB(0x51BAF7);

const Vec3 SC_TWILIGHT = Vec3(1, 0.5f, 0.01f);
const Vec3 AC_TWILIGHT = Vec3(0.6f, 0.5f, 0.5f);
const Vec3 YC_TWILIGHT = Vec3::fromRGB(0x463C53);

const Vec3 SC_NIGHT = Vec3(0.3f, 0.3f, 0.5f);
const Vec3 AC_NIGHT = Vec3(0.3f, 0.3f, 0.5f);
const Vec3 YC_NIGHT = Vec3(0.004f, 0.004f, 0.008f);

long deltaTime = 0;
Font font = Font("Arial");

int isqrt(int x) {
    assert(x >= 0);

    if (x == 0) {
        return 0;
    }

    for (int i = 1; i <= (x + 1) / 2; ++i) {
        if (i * i > x) {
            return i - 1;
        }
    }

    assert(false);
}

Fix16 playerX = Fix16(WORLD_SIZE + WORLD_SIZE / 2 + 0.5f - 5);
Fix16 playerY = fix16_from_int(WORLD_HEIGHT + 32 + 3); // more y means more down
Fix16 playerZ = playerX;

Fix16 velocityX = 0.0f;
Fix16 velocityY = 0.0f;
Fix16 velocityZ = 0.0f;

// mouse movement stuff
Point mouseLocation = Point();
Point lastMouseLocation = Point();
bool hovered = true;

int hoveredBlockPosX = -1;
int hoveredBlockPosY = -1;
int hoveredBlockPosZ = -1;

int placeBlockPosX = -1;
int placeBlockPosY = -1;
int placeBlockPosZ = -1;

int newHoverBlockPosX = -1;
int newHoverBlockPosY = -1;
int newHoverBlockPosZ = -1;

Vec3 lightDirection = Vec3(0.866025404f, -0.866025404f, 0.866025404f);

Fix16 cameraYaw = 0.0f;
Fix16 cameraPitch = 0.0f;
static const float FOV = 90.0f;

Fix16 sinYaw, sinPitch;
Fix16 cosYaw, cosPitch;

Vec3 sunColor = Vec3();
Vec3 ambColor = Vec3();
Vec3 skyColor = Vec3();

BlockId world[WORLD_SIZE][WORLD_HEIGHT][WORLD_SIZE] = {};

BufferedImage2 textureAtlasImage = BufferedImage2(TEXTURE_RES, 3 * 16 * TEXTURE_RES, BufferedImage2::TYPE_INT_ARGB);
Block *textureAtlas = textureAtlasImage.getDataBuffer();

BlockId hotbar[] = { BlockId::BLOCK_GRASS, BlockId::BLOCK_DEFAULT_DIRT, BlockId::BLOCK_STONE, BlockId::BLOCK_BRICKS, BlockId::BLOCK_WOOD, BlockId::BLOCK_LEAVES };
const size_t HOTBAR_LEN = sizeof(hotbar) / sizeof(BlockId);
int heldBlockIndex = 0;

Fix16 PI = 3.1415926535;

Fix16 scaled_cosine(Fix16 i) {
	return (Fix16) (Fix16(0.5f) * (Fix16(1.0f) - (i * PI).cos()));
}

void fillBox(BlockId blockId, int x0, int y0, int z0,
	int x1, int y1, int z1, bool replace)
{
	for(int x = x0; x < x1; x++)
	{
		for(int y = y0; y < y1; y++)
		{
			for(int z = z0; z < z1; z++)
			{
				if(!replace) {
					if(world[x][y][z] != BlockId::BLOCK_AIR)
						continue;
				}
				
				world[x][y][z] = blockId;
			}
		}
	}
}

static Fix16 perlin[PERLIN_RES + 1] = { 0.0 };
static bool perlin_is_init = false;

static const int perlin_octaves = 4; // default to medium smooth
static const Fix16 perlin_amp_falloff = 0.5f; // 50% reduction/octave
static const int PERLIN_YWRAPB = 4;
static const int PERLIN_YWRAP = 1 << PERLIN_YWRAPB;
static const int PERLIN_ZWRAPB = 8;
static const int PERLIN_ZWRAP = 1 << PERLIN_ZWRAPB;

Fix16 noise(Fix16 x, Fix16 y) { // stolen from Processing
    if (!perlin_is_init) {
        perlin_is_init = true;

        Random r = Random(18295169L);
        
        for (int i = 0; i < PERLIN_RES + 1; i++) {
            perlin[i] = r.nextFloat();
        }
    }

    if (x < 0)
        x = Fix16(0) - x;
    if (y < 0)
        y = Fix16(0) - y;
    
    int xi = (int16_t) x;
    int yi = (int16_t) y;
    
    Fix16 xf = x - xi;
    Fix16 yf = y - yi;
    Fix16 rxf, ryf;

    Fix16 r = 0;
    Fix16 ampl = 0.5f;

    Fix16 n1, n2, n3;

    for (int i = 0; i < perlin_octaves; i++) {
        int of = xi + (yi << PERLIN_YWRAPB);
        
        rxf = scaled_cosine(xf);
        ryf = scaled_cosine(yf);
        
        n1 = perlin[of % PERLIN_RES];
        n1 += rxf * (perlin[(of + 1) % PERLIN_RES] - n1);
        n2 = perlin[(of + PERLIN_YWRAP) % PERLIN_RES];
        n2 += rxf * (perlin[(of + PERLIN_YWRAP + 1) % PERLIN_RES] - n2);
        n1 += ryf * (n2 - n1);

        of += PERLIN_ZWRAP;
        n2 = perlin[of % PERLIN_RES];
        n2 += rxf * (perlin[(of + 1) % PERLIN_RES] - n2);
        n3 = perlin[(of + PERLIN_YWRAP) % PERLIN_RES];
        n3 += rxf * (perlin[(of + PERLIN_YWRAP + 1) % PERLIN_RES] - n3);
        n2 += ryf * (n3 - n2);

        n1 += scaled_cosine(0) * (n2 - n1);

        r += n1 * ampl;
        ampl *= perlin_amp_falloff;
        xi <<= 1;
        xf *= 2;
        yi <<= 1;
        yf *= 2;

        if (xf >= 1.0) {
            xi++;
            xf -= Fix16(fix16_from_int(1));
        }
        
        if (yf >= 1.0) {
            yi++;
            yf -= Fix16(fix16_from_int(1));
        }
    }

#ifndef HOSTED
    sleep();
#endif
    
    return r;
}

Fix16 fixgetposfrac(Fix16 x) {
    int32_t res = ((int32_t)x) % fix16_one;
    if (res < 0) {
        res += fix16_one;
    }
    return res;
}

Fix16 fixmod(Fix16 a, Fix16 b) {
    Fix16 r2 = (a / b);
    int16_t r = r2;
    if (r2 < 0.0 && Fix16((int16_t)r2) != r2) {
        r -= 1;
    }

    return a - b * Fix16(r);
}

Fix16 fixabs(Fix16 x) {
    if ((int32_t)x < 0) {
        return Fix16(0) - x;
    } else {
        return x;
    }
}

Fix16 fixmax(Fix16 a, Fix16 b) {
    if (a > b) {
        return a;
    } else {
        return b;
    }
}

Fix16 fixmin(Fix16 a, Fix16 b) {
    if (a < b) {
        return a;
    } else {
        return b;
    }
}

// https://gist.github.com/fairlight1337/4935ae72bcbcc1ba5c72`

void RGBtoHSV(Fix16 fR, Fix16 fG, Fix16 fB, Fix16& fH, Fix16& fS, Fix16& fV) {
  Fix16 fCMax = fixmax(fixmax(fR, fG), fB);
  Fix16 fCMin = fixmin(fixmin(fR, fG), fB);
  Fix16 fDelta = fCMax - fCMin;
  
  if(fDelta > 0) {
    if(fCMax == fR) {
      fH = Fix16(fix16_from_int(60)) * (fixmod(((fG - fB) / fDelta), (int16_t)6));
    } else if(fCMax == fG) {
      fH = Fix16(fix16_from_int(60)) * (((fB - fR) / fDelta) + (int16_t)2);
    } else if(fCMax == fB) {
      fH = Fix16(fix16_from_int(60)) * (((fR - fG) / fDelta) + (int16_t)4);
    }
    
    if(fCMax > 0) {
      fS = fDelta / fCMax;
    } else {
      fS = 0;
    }
    
    fV = fCMax;
  } else {
    fH = 0;
    fS = 0;
    fV = fCMax;
  }
  
  if(fH < 0) {
    fH = Fix16(fix16_from_int(360)) + fH;
  }
}

Block findNearestColor(int r, int g, int b, int a) {
    Block best = (Block)-1;
    int bestDiff = INT_MAX;

    if (a == 0) {
        return Block::AIR;
    }

    Fix16 h, s, v;
    RGBtoHSV(Fix16((int16_t)r) / 255.0, Fix16((int16_t)g) / 255.0, Fix16((int16_t)b) / 255.0, h, s, v);

    for (int bi = 0; bi < 14; ++bi) {
        Block bl = (Block)bi;

        if (bl == Block::AIR) {
            continue;
        }

        ColorRGBA blockColor = colorFromBlock(bl);
        Fix16 bh, bs, bv;
        RGBtoHSV(Fix16((int16_t)blockColor.r) / 255.0, Fix16((int16_t)blockColor.g) / 255.0, Fix16((int16_t)blockColor.b) / 255.0, bh, bs, bv);

        ColorRGBA color = colorFromBlock(bl);

        int diff = fixabs(bh - h);

        if (diff < bestDiff) {
            bestDiff = diff;
            best = bl;
        }
    }
    assert(best != (Block)-1);

    if (best == Block::LAPIS_BLOCK) {
        assert(false);
    }

    return best;
}

#ifdef HOSTED
#include <stdio.h>
#endif

Fix16 mulFrustAng(Fix16 frust, Fix16 ang) {
    //assert((int16_t)fixabs(frust) <= 3);
    //assert((int16_t)fixabs(ang) <= 1);

    return ((((int32_t)frust) >> 2) * ((int32_t)ang)) >> 14;
}

Fix16 fixrecipsmall(Fix16 x) {
    if (-2 <= (int32_t)x && (int32_t)x <= 0) {
        return INT_MIN;
    } else if (0 < (int32_t)x && (int32_t)x <= 2) {
        return INT_MAX;
    } else {
        int32_t num = 1 << 30;
        return (num / (int32_t)x) << 2;
    }
}

/* temp = cosPitch + mulFrustAng(frustrumRayY, sinPitch) */

Block renderPixel(
    int screenIndex,
    Fix16 frustRayYCosPitchNSinPitch, /* = mulFrustAng(frustrumRayY, cosPitch) - sinPitch */
    Fix16 tempSinYaw, /* = mulFrustAng(temp, sinYaw) */
    Fix16 tempCosYaw  /* = mulFrustAng(temp, cosYaw) */
) {
    int screenX = screenIndex % SCR_RES_X;
    int screenY = screenIndex / SCR_RES_X;

    int screenXMid = (screenX - (SCR_RES_X / 2));
    //int screenYMid = (screenY - (SCR_RES_Y / 2));

    assert((SCR_RES_X / 2) * 214 <= 32700);

    /* Both of these are <= 3 */
    Fix16 frustrumRayX = (int32_t)((screenXMid * 214 * fix16_one) / (SCR_RES_X * (int)FOV));
    //Fix16 frustrumRayY = (int32_t)((screenYMid * 120 * fix16_one) / (SCR_RES_Y * (int)FOV));

    //Fix16 frustrumRayX = Fix16((int16_t)screenXMid) * Fix16(214 / (SCR_RES_X * FOV));
    //Fix16 frustrumRayY = Fix16((int16_t)screenYMid) * Fix16(120 / (SCR_RES_Y * FOV));

    // rotate frustum space to world space
    //Fix16 temp = cosPitch + mulFrustAng(frustrumRayY, sinPitch);

    //Vec3 rayDir = Vec3(mulFrustAng(frustrumRayX, cosYaw) + mulFrustAng(temp, sinYaw), mulFrustAng(frustrumRayY, cosPitch) - sinPitch, mulFrustAng(temp, cosYaw) - mulFrustAng(frustrumRayX, sinYaw));
    Vec3 rayDir = Vec3(mulFrustAng(frustrumRayX, cosYaw) + tempSinYaw, frustRayYCosPitchNSinPitch, tempCosYaw - mulFrustAng(frustrumRayX, sinYaw));
    rayDir = Vec3::normalize_small(rayDir);
    
    Fix16 furthestHit = RENDER_DIST;
    
    Fix16 hoverCheckDist = PLAYER_REACH; // start at max reach

    Fix16 rayOriginX = playerX;
    Fix16 rayOriginY = playerY;
    Fix16 rayOriginZ = playerZ;
    
    int stepX = (rayDir.x > 0) ? 1 : -1;
    int stepY = (rayDir.y > 0) ? 1 : -1;
    int stepZ = (rayDir.z > 0) ? 1 : -1;

    Fix16 xEdgeOffset = fixgetposfrac(rayOriginX);
    if (stepX > 0) { xEdgeOffset = Fix16(1.0) - xEdgeOffset; };
    Fix16 yEdgeOffset = fixgetposfrac(rayOriginY);
    if (stepY > 0) { yEdgeOffset = Fix16(1.0) - yEdgeOffset; };
    Fix16 zEdgeOffset = fixgetposfrac(rayOriginZ);
    if (stepZ > 0) { zEdgeOffset = Fix16(1.0) - zEdgeOffset; };

    if (fixabs(rayDir.x) <= 1) { rayDir.x = ((int32_t)rayDir.x > 0) ? 2 : -2; }
    if (fixabs(rayDir.y) <= 1) { rayDir.y = ((int32_t)rayDir.y > 0) ? 2 : -2; }
    if (fixabs(rayDir.z) <= 1) { rayDir.z = ((int32_t)rayDir.z > 0) ? 2 : -2; }

    Fix16 tDeltaX = fixrecipsmall(fixabs(rayDir.x));
    Fix16 tDeltaY = fixrecipsmall(fixabs(rayDir.y));
    Fix16 tDeltaZ = fixrecipsmall(fixabs(rayDir.z));

    Fix16 tMaxX = xEdgeOffset * tDeltaX;
    Fix16 tMaxY = yEdgeOffset * tDeltaY;
    Fix16 tMaxZ = zEdgeOffset * tDeltaZ;

    Fix16 rayDist = 0.0;

    int blockX = (int16_t)rayOriginX - WORLD_SIZE;
    int blockY = (int16_t)rayOriginY - WORLD_HEIGHT;
    int blockZ = (int16_t)rayOriginZ - WORLD_SIZE;

#ifdef HOSTED
    if (screenIndex == 0) {
        std::cout << "Pos: " << (float)rayOriginX << ", " << (float)rayOriginY << ", " << (float)rayOriginZ << "\n";
        std::cout << "Dir: " << (float)rayDir.x << ", " << (float)rayDir.y << ", " << (float)rayDir.z << "\n";
        std::cout << "Offset: " << (float)xEdgeOffset << ", " << (float)yEdgeOffset << ", " << (float)zEdgeOffset << "\n";
    }
#endif

    Axis lastAxis = Axis::X;

    if (blockX < 0 || blockX >= WORLD_SIZE) {
        return Block::DIAMOND_BLOCK;
    }
    if (blockY < 0 || blockY >= WORLD_HEIGHT) {
        return Block::DIAMOND_BLOCK;
    }
    if (blockZ < 0 || blockZ >= WORLD_SIZE) {
        return Block::DIAMOND_BLOCK;
    }

    while (1) {
        BlockId hitBlock = world[blockX][blockY][blockZ];

        if (hitBlock != BlockId::BLOCK_AIR) {
            Fix16 rayX = rayOriginX + rayDist * rayDir.x;
            Fix16 rayY = rayOriginY + rayDist * rayDir.y;
            Fix16 rayZ = rayOriginZ + rayDist * rayDir.z;

            assert(TEXTURE_RES == 16);

            //int texFetchX = (int16_t)((rayX + rayZ) * (int16_t)TEXTURE_RES) % TEXTURE_RES;
            int texFetchX = (int32_t(rayX + rayZ) >> 12) % TEXTURE_RES;
            //int texFetchY = ((int16_t)(rayY * (int16_t)TEXTURE_RES) % TEXTURE_RES) + TEXTURE_RES;
            int texFetchY = ((int32_t(rayY) >> 12) % TEXTURE_RES) + TEXTURE_RES;

            if (lastAxis == Axis::Y)
            {
                //texFetchX = (int16_t)(rayX * (int16_t)TEXTURE_RES) % TEXTURE_RES;
                //texFetchY = (int16_t)(rayZ * (int16_t)TEXTURE_RES) % TEXTURE_RES;
                texFetchX = (int32_t(rayX) >> 12) % TEXTURE_RES;
                texFetchY = (int32_t(rayZ) >> 12) % TEXTURE_RES;

                if (stepY < 0) // looking at the underside of a block
                    texFetchY += TEXTURE_RES * 2;
            }

            Block textureColor;
            if (blockX == hoveredBlockPosX && blockY == hoveredBlockPosY && blockZ == hoveredBlockPosZ &&
                    (  (texFetchX == 0               || texFetchY % TEXTURE_RES == 0)
                    || (texFetchX == TEXTURE_RES - 1 || texFetchY % TEXTURE_RES == TEXTURE_RES - 1)))
                //textureColor = Vec3((int16_t)1, (int16_t)1, (int16_t)1); // add white outline to hovered block
                textureColor = Block::IRON_BLOCK;
            else
                textureColor = textureAtlas[texFetchX + texFetchY * TEXTURE_RES + static_cast<int>(hitBlock) * (TEXTURE_RES * TEXTURE_RES) * 3];

            if (rayDist < hoverCheckDist && screenX == (SCR_RES_X * 2) / 4 && screenY == (SCR_RES_Y * 2) / 4) {
                newHoverBlockPosX = blockX;
                newHoverBlockPosY = blockY;
                newHoverBlockPosZ = blockZ;

                placeBlockPosX = 0;
                placeBlockPosY = 0;
                placeBlockPosZ = 0;

                switch(lastAxis) {
                    case Axis::X:
                        placeBlockPosX = stepX;
                        break;
                    case Axis::Y:
                        placeBlockPosY = stepY;
                        break;
                    case Axis::Z:
                        placeBlockPosZ = stepZ;
                }

                hoverCheckDist = rayDist;
            }

            if (textureColor != Block::AIR) {
                furthestHit = rayDist;

                return textureColor;
            }
        }

        if (tMaxX < tMaxY) {
            if (tMaxX < tMaxZ) {
                blockX += stepX;
                if (blockX < 0 || blockX >= WORLD_SIZE) {
                    break;
                }
                rayDist = tMaxX;
                tMaxX += tDeltaX;
                lastAxis = Axis::X;
            } else {
                blockZ += stepZ;
                if (blockZ < 0 || blockZ >= WORLD_SIZE) {
                    break;
                }
                rayDist = tMaxZ;
                tMaxZ += tDeltaZ;
                lastAxis = Axis::Z;
            }
        } else {
            if (tMaxY < tMaxZ) {
                blockY += stepY;
                if (blockY < 0 || blockY >= WORLD_HEIGHT) {
                    break;
                }
                rayDist = tMaxY;
                tMaxY += tDeltaY;
                lastAxis = Axis::Y;
            } else {
                blockZ += stepZ;
                if (blockZ < 0 || blockZ >= WORLD_SIZE) {
                    break;
                }
                rayDist = tMaxZ;
                tMaxZ += tDeltaZ;
                lastAxis = Axis::Z;
            }
        }

        if (rayDist > RENDER_DIST) {
            break;
        }
    }

    return Block::DIAMOND_BLOCK;
}

#if 0

Vec3 renderPixelVec2(int screenIndex) {
    int screenX = screenIndex % SCR_RES_X;
    int screenY = screenIndex / SCR_RES_X;

    int16_t screenXMid = (screenX - (SCR_RES_X / 2));
    int16_t screenYMid = (screenY - (SCR_RES_Y / 2));

    Fix16 frustumRayX = Fix16(screenXMid) * Fix16(214.0 / (SCR_RES_X * FOV));
    Fix16 frustumRayY = Fix16(screenYMid) * Fix16(120.0 / (SCR_RES_Y * FOV));

    // rotate frustum space to world space
    Fix16 temp = cosPitch + frustumRayY * sinPitch;

    Vec3 rayDir = Vec3(frustumRayX * cosYaw + temp * sinYaw, frustumRayY * cosPitch - sinPitch, temp * cosYaw - frustumRayX * sinYaw);
    Vec3::normalize(rayDir, rayDir);
    
    Fix16 sunDot = Vec3::dot(rayDir, Vec3(lightDirection.x, lightDirection.y, lightDirection.z));
    
    Vec3 pixelColor = Vec3::fromRGB(0x86, 0xB2, 0xFE); // sky color (default)
    Fix16 lightIntensity = 1.0f;
    
    Fix16 furthestHit = RENDER_DIST;
    
    Fix16 hoverCheckDist = PLAYER_REACH; // start at max reach

    Fix16 closestHitX = (int16_t)-1;
    Fix16 closestHitY = (int16_t)-1;
    Fix16 closestHitZ = (int16_t)-1;
    
    Fix16 rayOriginX = playerX;
    Fix16 rayOriginY = playerY;
    Fix16 rayOriginZ = playerZ;
    
    bool blockHit = false;

    const int FIRST_PIXEL = 956;

    for(int pass = 0; pass < (classicRender ? 1 : 2); pass++)
    {
        for (int axis_idx = 0; axis_idx < 3; axis_idx++)
        {
            if (screenIndex == FIRST_PIXEL) {
                print(7000 + axis_idx);
            }

            Axis axis = static_cast<Axis>(axis_idx);
            // align ray to block edge on this axis
            // and calc ray deltas
            Fix16 delta;

            Fix16 rayDeltaX, rayDeltaY, rayDeltaZ;

            switch(axis)
            {
                default:
                case Axis::X:
                    delta = rayDir.x;
                    /*rayDeltaX = (rayDir.x < 0) ? -1.0 : 1.0;
                    rayDeltaY = rayDir.y / fixabs(delta);
                    rayDeltaZ = rayDir.z / fixabs(delta);*/
                    break;
                case Axis::Y:
                    delta = rayDir.y;
                    /*rayDeltaX = rayDir.x / fixabs(delta);
                    rayDeltaY = (rayDir.y < 0) ? -1.0 : 1.0;
                    rayDeltaZ = rayDir.z / fixabs(delta);*/
                    break;
                case Axis::Z:
                    delta = rayDir.z;
                    /*rayDeltaX = rayDir.x / fixabs(delta);
                    rayDeltaY = rayDir.y / fixabs(delta);
                    rayDeltaZ = (rayDir.z < 0) ? -1.0 : 1.0;*/
                    break;
            }


            /*float rayDeltaX = rayDir.x / (float)fabs(delta);
            float rayDeltaY = rayDir.y / (float)fabs(delta);
            float rayDeltaZ = rayDir.z / (float)fabs(delta);*/

            rayDeltaX = rayDir.x / fixabs(delta);
            rayDeltaY = rayDir.y / fixabs(delta);
            rayDeltaZ = rayDir.z / fixabs(delta);

            if (screenIndex == FIRST_PIXEL) {
                print((int32_t)delta);
                print((int32_t)rayDeltaX);
                print((int32_t)rayDeltaY);
                print((int32_t)rayDeltaZ);
            }

            Fix16 playerOffsetFromBlockEdge; // TODO confirm
            switch(axis)
            {
                default:
                case Axis::X:
                    playerOffsetFromBlockEdge = fixmod(rayOriginX, 1.0);
                    break;
                case Axis::Y:
                    playerOffsetFromBlockEdge = fixmod(rayOriginY, 1.0);
                    break;
                case Axis::Z:
                    playerOffsetFromBlockEdge = fixmod(rayOriginZ, 1.0);
                    break;
            }

            if (screenIndex == FIRST_PIXEL) {
                print((int32_t)rayOriginX);
                print((int32_t)playerOffsetFromBlockEdge);
            }

            if (delta > 0)
                playerOffsetFromBlockEdge = Fix16(1.0) - playerOffsetFromBlockEdge;

            if (screenIndex == FIRST_PIXEL) {
                print((int32_t)playerOffsetFromBlockEdge);
            }

            Fix16 rayTravelDist = playerOffsetFromBlockEdge / fixabs(delta);

            if (screenIndex == FIRST_PIXEL) {
                print((int32_t)rayTravelDist);
            }

            Fix16 rayX = Fix16(rayOriginX) + rayDeltaX * playerOffsetFromBlockEdge;
            Fix16 rayY = Fix16(rayOriginY) + rayDeltaY * playerOffsetFromBlockEdge;
            Fix16 rayZ = Fix16(rayOriginZ) + rayDeltaZ * playerOffsetFromBlockEdge;

            if (delta < 0.0F)
            {
                if (axis == Axis::X)
                    rayX -= Fix16((int16_t)1);

                if (axis == Axis::Y)
                    rayY -= Fix16((int16_t)1);

                if (axis == Axis::Z)
                    rayZ -= Fix16((int16_t)1);
            }
            
            // do the raycast
            while (rayTravelDist < furthestHit)
            {
                if (screenIndex == FIRST_PIXEL) {
                    print(8000);

                    print((int32_t)rayX);
                    print((int32_t)rayY);
                    print((int32_t)rayZ);
                }

                int blockHitX = ((int16_t)rayX + (rayX < 0)) - WORLD_SIZE;
                int blockHitY = ((int16_t)rayY + (rayY < 0)) - WORLD_HEIGHT;
                int blockHitZ = ((int16_t)rayZ + (rayZ < 0)) - WORLD_SIZE;

                // if ray exits the world
                if (blockHitX < 0 || blockHitY < -2 || blockHitZ < 0 || blockHitX >= WORLD_SIZE || blockHitY >= WORLD_HEIGHT || blockHitZ >= WORLD_SIZE)
                    break;

                BlockId blockHitID = blockHitY < 0 ? BlockId::BLOCK_AIR : world[blockHitX][blockHitY][blockHitZ];

                if (blockHitID != BlockId::BLOCK_AIR)
                {
                    int texFetchX = (int16_t)((rayX + rayZ) * (int16_t)TEXTURE_RES) % TEXTURE_RES;
                    int texFetchY = ((int16_t)(rayY * (int16_t)TEXTURE_RES) % TEXTURE_RES) + TEXTURE_RES;

                    if (axis == Axis::Y)
                    {
                        texFetchX = (int16_t)(rayX * (int16_t)TEXTURE_RES) % TEXTURE_RES;
                        texFetchY = (int16_t)(rayZ * (int16_t)TEXTURE_RES) % TEXTURE_RES;

                        if (rayDeltaY < 0.0F) // looking at the underside of a block
                            texFetchY += TEXTURE_RES * 2;
                    }

                    Vec3 textureColor;
                    if(pass == 0 &&
                            (blockHitX == hoveredBlockPosX && blockHitY == hoveredBlockPosY && blockHitZ == hoveredBlockPosZ &&
                            (  (texFetchX == 0               || texFetchY % TEXTURE_RES == 0)
                            || (texFetchX == TEXTURE_RES - 1 || texFetchY % TEXTURE_RES == TEXTURE_RES - 1))))
                        textureColor = Vec3((int16_t)1, (int16_t)1, (int16_t)1); // add white outline to hovered block
                    else
                        textureColor = Vec3::fromRGB(textureAtlas[texFetchX + texFetchY * TEXTURE_RES + static_cast<int>(blockHitID) * (TEXTURE_RES * TEXTURE_RES) * 3]);

                    int direction = 1;
                    if (delta > 0.0F)
                        direction = -1;
                    
                    if (rayTravelDist < hoverCheckDist && screenX == (SCR_RES_X * 2) / 4 && screenY == (SCR_RES_Y * 2) / 4) {
                        if(pass == 0)
                        {
                            newHoverBlockPosX = blockHitX;
                            newHoverBlockPosY = blockHitY;
                            newHoverBlockPosZ = blockHitZ;

                            placeBlockPosX = 0;
                            placeBlockPosY = 0;
                            placeBlockPosZ = 0;

                            switch(axis) {
                                case Axis::X:
                                    placeBlockPosX = direction;
                                    break;
                                case Axis::Y:
                                    placeBlockPosY = direction;
                                    break;
                                case Axis::Z:
                                    placeBlockPosZ = direction;
                            }
                        }

                        hoverCheckDist = rayTravelDist;
                    }

                    if (!textureColor.isZero()) {
                        if(pass == 0) // not shadows
                        {
                            pixelColor = textureColor;
                        }
                        else if(!classicRender) { // shadows
                            lightIntensity = 0.25f;
                            
                            goto AXIS;
                        }
                        
                        furthestHit = rayTravelDist;

                        if(!classicRender)
                        {
                            closestHitX = rayX;
                            closestHitY = rayY;
                            closestHitZ = rayZ;
                            
                            switch(axis)
                            {
                                case Axis::X:
                                    lightIntensity = Fix16((int16_t)direction) * lightDirection.x;
                                    break;
                                case Axis::Y:
                                    lightIntensity = Fix16((int16_t)direction) * lightDirection.y;
                                    break;
                                case Axis::Z:
                                    lightIntensity = Fix16((int16_t)direction) * lightDirection.z;
                                    break;
                            }

                            lightIntensity = (Fix16((int16_t)1) + lightIntensity) / 2.0F;
                        }
                        
                        blockHit = true;
                    }
                }

                rayX += rayDeltaX;
                rayY += rayDeltaY;
                rayZ += rayDeltaZ;

                rayTravelDist += Fix16(1.0) / fixabs(delta);
            }

#ifndef HOSTED
            sleep();
#endif
        }

        AXIS:
        
        if(!blockHit) // don't do sky shadows
            break;
        
        // TODO why?
        if(lightIntensity <= 0.5f)
            break;
        
        // prepare for shadows pass
        rayOriginX = closestHitX;
        rayOriginY = closestHitY;
        rayOriginZ = closestHitZ;
        
        rayDir.x = lightDirection.x;
        rayDir.y = lightDirection.y;
        rayDir.z = lightDirection.z;
        
        furthestHit = RENDER_DIST;
    }
    
    return pixelColor;
}

#endif

void render()
{
    int screenIndex = 0;

    for (int y = 0; y < SCR_RES_Y; ++y) {
        int screenY = screenIndex / SCR_RES_X;
        int screenYMid = (screenY - (SCR_RES_Y / 2));
        Fix16 frustrumRayY = (int32_t)((screenYMid * 120 * fix16_one) / (SCR_RES_Y * (int)FOV));
        Fix16 temp = cosPitch + mulFrustAng(frustrumRayY, sinPitch);
        Fix16 frustRayYCosPitchNSinPitch = mulFrustAng(frustrumRayY, cosPitch) - sinPitch;
        Fix16 tempSinYaw = mulFrustAng(temp, sinYaw);
        Fix16 tempCosYaw = mulFrustAng(temp, cosYaw);

        for (int x = 0; x < SCR_RES_X; ++x) {


            Block pixelColor = renderPixel(
                screenIndex++,
                frustRayYCosPitchNSinPitch,
                tempSinYaw,
                tempCosYaw);

            setPixel(x, y, pixelColor);

#ifndef HOSTED
            if (screenIndex % 32 == 0) {
                sleep();
            }
#endif
        }
    }
}

void mouseWheelMoved(MouseWheelEvent e) {
    if(e.getUnitsToScroll() < 0)
        heldBlockIndex--;
    else
        heldBlockIndex++;
    
    if(heldBlockIndex < 0)
        heldBlockIndex = HOTBAR_LEN - 1;
    
    if(heldBlockIndex >= HOTBAR_LEN)
        heldBlockIndex = 0;
}

void genTerrain() {
    Random rand = Random(18295169L);

    // generate world

    print(1000);
    
    Fix16 maxTerrainHeight = WORLD_HEIGHT / 2.0;
    if (classicTerrain) {
        for (int x = WORLD_SIZE; x >= 0; x--) {
            print(2000 + x);

            for(int y = 0; y < WORLD_HEIGHT; y++) {
                for(int z = 0; z < WORLD_SIZE; z++) {
                    BlockId block;

                    if(y > (int16_t)maxTerrainHeight + rand.nextInt(8))
                        block = (BlockId) (rand.nextInt(8) + 1);
                    else
                        block = BlockId::BLOCK_AIR;

                    if(x == WORLD_SIZE)
                        continue;

                    world[x][y][z] = block;
                }
            }

#ifndef HOSTED
            sleep();
#endif
        }
    } else {
        Fix16 halfWorldSize = WORLD_SIZE / 2.0;
        
        const int stoneDepth = 5;
        
        for (int x = 0; x < WORLD_SIZE; x++) {
            print(2000 + x);
            for(int z = 0; z < WORLD_SIZE; z++) {
                int terrainHeight = (int16_t)(maxTerrainHeight + noise(Fix16((int16_t)x) / halfWorldSize, Fix16((int16_t)z) / halfWorldSize) * 10.0);
                
                for(int y = terrainHeight; y < WORLD_HEIGHT; y++)
                {
                    BlockId block;
                    
                    if(y > terrainHeight + stoneDepth)
                        block = BlockId::BLOCK_STONE;
                    else if (y > terrainHeight)
                        block = BlockId::BLOCK_DEFAULT_DIRT;
                    else // (y == terrainHeight)
                        block = BlockId::BLOCK_GRASS;

                    world[x][y][z] = block;
                }
            }
#ifndef HOSTED
            sleep();
#endif
        }
#ifndef HOSTED
        sleep();
#endif

        // populate trees
        for (int x = 4; x < WORLD_SIZE - 4; x += 8) {
            for(int z = 4; z < WORLD_SIZE - 4; z += 8) {
                if(rand.nextInt(4) == 0) // spawn tree
                {
                    int treeX = x + (rand.nextInt(4) - 2);
                    int treeZ = z + (rand.nextInt(4) - 2);
                    
                    const int terrainHeight = (int16_t)(maxTerrainHeight + noise(Fix16((int16_t)treeX) / halfWorldSize, Fix16((int16_t)treeZ) / halfWorldSize) * fix16_from_int(10)) - 1;
                    
                    int treeHeight = 4 + rand.nextInt(2); // min 4 max 5
                    
                    for(int y = terrainHeight; y >= terrainHeight - treeHeight; y--)
                    {
                        BlockId block = BlockId::BLOCK_WOOD;
                        
                        world[treeX][y][treeZ] = block;
                    }
                    
                    // foliage
                    fillBox(BlockId::BLOCK_LEAVES, treeX - 2, terrainHeight - treeHeight + 1, treeZ - 2, treeX + 3, terrainHeight - treeHeight + 3, treeZ + 3, false);
                    
                    // crown
                    fillBox(BlockId::BLOCK_LEAVES, treeX - 1, terrainHeight - treeHeight - 1, treeZ - 1, treeX + 2, terrainHeight - treeHeight + 1, treeZ + 2, false);
                    
                    int foliageXList[] = { treeX - 2, treeX - 2, treeX + 2, treeX + 2 };
                    int foliageZList[] = { treeZ - 2, treeZ + 2, treeZ + 2, treeZ - 2 };
                    
                    int crownXList[] = { treeX - 1, treeX - 1, treeX + 1, treeX + 1 };
                    int crownZList[] = { treeZ - 1, treeZ + 1, treeZ + 1, treeZ - 1 };
                    
                    for (int i = 0; i < 4; i++)
                    {
                        int foliageX = foliageXList[i];
                        int foliageZ = foliageZList[i];
                        
                        int foliageCut = rand.nextInt(10);
                        
                        switch(foliageCut) {
                            case 0: // cut out top
                                world[foliageX][terrainHeight - treeHeight + 1][foliageZ] = BlockId::BLOCK_AIR;
                                break;
                            case 1: // cut out bottom
                                world[foliageX][terrainHeight - treeHeight + 2][foliageZ] = BlockId::BLOCK_AIR;
                                break;
                            case 2: // cut out both
                                world[foliageX][terrainHeight - treeHeight + 1][foliageZ] = BlockId::BLOCK_AIR;
                                world[foliageX][terrainHeight - treeHeight + 2][foliageZ] = BlockId::BLOCK_AIR;
                                break;
                            default: // do nothing
                                break;
                        }
                        
                        
                        int crownX = crownXList[i];
                        int crownZ = crownZList[i];
                        
                        int crownCut = rand.nextInt(10);
                        
                        switch(crownCut) {
                            case 0: // cut out both
                                world[crownX][terrainHeight - treeHeight - 1][crownZ] = BlockId::BLOCK_AIR;
                                world[crownX][terrainHeight - treeHeight][crownZ] = BlockId::BLOCK_AIR;
                                break;
                            default: // do nothing
                                world[crownX][terrainHeight - treeHeight - 1][crownZ] = BlockId::BLOCK_AIR;
                                break;
                        }
                    }
                }
            }
        }

/*
#ifdef HOSTED
        std::cout << "{\n";
        for (int x = 0; x < WORLD_SIZE; ++x) {
            std::cout << "  {\n";
            for (int y = 0; y < WORLD_HEIGHT; ++y) {
                std::cout << "    { ";
                for (int z = 0; z < WORLD_SIZE; ++z) {
                    switch (world[x][y][z]) {
                        case BlockId::BLOCK_AIR: std::cout << "A"; break;
                        case BlockId::BLOCK_GRASS: std::cout << "G"; break;
                        case BlockId::BLOCK_DEFAULT_DIRT: std::cout << "D"; break;
                        case BlockId::BLOCK_STONE: std::cout << "S"; break;
                        case BlockId::BLOCK_WOOD: std::cout << "W"; break;
                        case BlockId::BLOCK_LEAVES: std::cout << "L"; break;
                        default: std::cerr << "OHNO " << (int)world[x][y][z] << "\n"; assert(false); break;
                    }

                    if (z != WORLD_SIZE - 1) {
                        std::cout << ", ";
                    } else {
                        std::cout << " ";
                    }
                }
                std::cout << "}";
                if (y != WORLD_HEIGHT - 1) {
                    std::cout << ",";
                }
                std::cout << "\n";
            }

            std::cout << "  }";
            if (x != WORLD_SIZE - 1) {
                std::cout << ", ";
            }
            std::cout << "\n";
        }
        std::cout << "}\n";

        std::cout << std::endl;

        assert(false);
#endif
*/
    }
}

class Minecraft4k {
public:
    void run() {
        genTerrain();

        genTextures();

        runLoop();
    }
    
    void genTextures() {
        Random rand = Random(151910774187927L);
        
        // procedually generates the 16x3 textureAtlas
        // gsd = grayscale detail
        for (int blockTypeIdx = 1; blockTypeIdx < 16; blockTypeIdx++) {
            BlockId blockType = static_cast<BlockId>(blockTypeIdx);

            print(5000 + blockTypeIdx);

            int gsd_tempA = 0xFF - rand.nextInt(0x60);

            if (!classicTextures && blockType == BlockId::BLOCK_STONE) {
                Block *stone_ptr = textureAtlas + static_cast<int>(BlockId::BLOCK_STONE) * (TEXTURE_RES * TEXTURE_RES) * 3;
                size_t stone_len = (TEXTURE_RES * TEXTURE_RES) * 3;
                for (int i = 0; i < stone_len; ++i) {
                    stone_ptr[i] = Block::COBBLESTONE;
                }
                continue;
            }

            if (!classicTextures && blockType == BlockId::BLOCK_WOOD) {
                Block *wood_ptr = textureAtlas + static_cast<int>(BlockId::BLOCK_WOOD) * (TEXTURE_RES * TEXTURE_RES) * 3;
                size_t wood_len = (TEXTURE_RES * TEXTURE_RES) * 3;
                for (int i = 0; i < wood_len; ++i) {
                    wood_ptr[i] = Block::OAK_LOG_BLOCK;
                }
                continue;
            }

            // TODO: Remove

            if (!classicTextures && blockType == BlockId::BLOCK_BRICKS) {
                Block *brick_ptr = textureAtlas + static_cast<int>(BlockId::BLOCK_BRICKS) * (TEXTURE_RES * TEXTURE_RES) * 3;
                size_t brick_len = (TEXTURE_RES * TEXTURE_RES) * 3;
                for (int i = 0; i < brick_len; ++i) {
                    brick_ptr[i] = Block::REDSTONE_BLOCK;
                }
                continue;
            }

            /*if (!classicTextures && blockType == BlockId::BLOCK_LEAVES) {
                Block *brick_ptr = textureAtlas + static_cast<int>(BlockId::BLOCK_LEAVES) * (TEXTURE_RES * TEXTURE_RES) * 3;
                size_t brick_len = (TEXTURE_RES * TEXTURE_RES) * 3;
                for (int i = 0; i < brick_len; ++i) {
                    brick_ptr[i] = Block::EMERALD_BLOCK;
                }
                continue;
            }*/

            if (!classicTextures) {
                Block *other_ptr = textureAtlas + static_cast<int>(blockType) * (TEXTURE_RES * TEXTURE_RES) * 3;
                size_t other_len = (TEXTURE_RES * TEXTURE_RES) * 3;
                for (int i = 0; i < other_len; ++i) {
                    other_ptr[i] = Block::DIRT_BLOCK;
                }
            }

            if (blockType != BlockId::BLOCK_GRASS && blockType != BlockId::BLOCK_LEAVES && blockType != BlockId::BLOCK_BRICKS) {
                continue;
            }

            for (int y = 0; y < TEXTURE_RES * 3; y++) {
                print(6000 + y);

                for (int x = 0; x < TEXTURE_RES; x++) {
                    // gets executed per pixel/texel
                    
                    int gsd_const;
                    int tint;

                    if (classicTextures) {
                        if (blockType != BlockId::BLOCK_STONE || rand.nextInt(3) == 0) // if the block type is stone, update the noise value less often to get a streched out look
                            gsd_tempA = 0xFF - rand.nextInt(0x60);

                        tint = 0x966C4A; // brown (dirt)
                        switch (static_cast<int>(blockType))
                        {
                            case static_cast<int>(BlockId::BLOCK_STONE):
                                tint = 0x7F7F7F; // grey
                                break;
                            case static_cast<int>(BlockId::BLOCK_GRASS):
                                if (y < ((x * x * 3 + x * 81) >> 2 & 0x3) + TEXTURE_RES * 9 / 8) // grass + grass edge
                                    tint = 0x6AAA40; // green
                                else if (y < ((x * x * 3 + x * 81) >> 2 & 0x3) + TEXTURE_RES * 19 / 16) // grass edge shadow
                                    gsd_tempA = gsd_tempA * 2 / 3;
                                break;
                            case static_cast<int>(BlockId::BLOCK_WOOD):
                                tint = 0x675231; // brown (bark)
                                if(!(y >= TEXTURE_RES && y < TEXTURE_RES * 2) && // second row = stripes
                                        x > 0 && x < TEXTURE_RES - 1 &&
                                        ((y > 0 && y < TEXTURE_RES - 1) || (y > TEXTURE_RES * 2 && y < TEXTURE_RES * 3 - 1))) { // wood side area
                                    tint = 0xBC9862; // light brown

                                    // the following code repurposes 2 gsd variables making it a bit hard to read
                                    // but in short it gets the absulte distance from the tile's center in x and y direction 
                                    // finds the max of it
                                    // uses that to make the gray scale detail darker if the current pixel is part of an annual ring
                                    // and adds some noise as a finishig touch
                                    int woodCenter = TEXTURE_RES / 2 - 1;
                                    
                                    int dx = x - woodCenter;
                                    int dy = (y % TEXTURE_RES) - woodCenter;

                                    if (dx < 0)
                                        dx = 1 - dx;

                                    if (dy < 0)
                                        dy = 1 - dy;

                                    if (dy > dx)
                                        dx = dy;

                                    gsd_tempA = 196 - rand.nextInt(32) + dx % 3 * 32;
                                } else if (rand.nextInt(2) == 0) {
                                    // make the gsd 50% brighter on random pixels of the bark
                                    // and 50% darker if x happens to be odd
                                    gsd_tempA = gsd_tempA * (150 - (x & 1) * 100) / 100;
                                }
                                break;
                            case static_cast<int>(BlockId::BLOCK_BRICKS):
                                tint = 0xB53A15; // red
                                if ((x + y / 4 * 4) % 8 == 0 || y % 4 == 0) // gap between bricks
                                    tint = 0xBCAFA5; // reddish light grey
                                break;
                            default:
                                break;
                        }

                        gsd_const = gsd_tempA;
                        if (y >= TEXTURE_RES * 2) // bottom side of the block
                            gsd_const /= 2; // make it darker, baked "shading"

                        if (blockType == BlockId::BLOCK_LEAVES) {
                            //tint = 0x50D937; // green
                            tint = 0x50FF37;
                            if (rand.nextInt(2) == 0) {
                                tint = 0;
                                gsd_const = 0xFF;
                            }
                        }

                        // multiply tint by the grayscale detail
                        int a = ((tint & 0xFFFFFF) == 0 ? 0 : 0xFF);
                        int r = (tint >> 16 & 0xFF) * gsd_const / 0xFF;
                        int g = (tint >>  8 & 0xFF) * gsd_const / 0xFF;
                        int b = (tint       & 0xFF) * gsd_const / 0xFF;
                        
                        Block col2 = findNearestColor(r, g, b, a);

                        // write pixel to the texture atlas
                        textureAtlas[x + y * TEXTURE_RES + static_cast<int>(blockType) * (TEXTURE_RES * TEXTURE_RES) * 3] = col2;

                    } else {
                        switch (static_cast<int>(blockType))
                        {
                            case static_cast<int>(BlockId::BLOCK_GRASS):
                                if (y < ((x * x * 3 + x * 81) >> 2 & 0x3) + TEXTURE_RES * 9 / 8) { // grass + grass edge
                                    tint = 0x6AAA40; // green
                                    textureAtlas[x + y * TEXTURE_RES + static_cast<int>(blockType) * (TEXTURE_RES * TEXTURE_RES) * 3] = Block::OAK_LEAVES_BLOCK;
                                    continue;
                                } else if (y < ((x * x * 3 + x * 81) >> 2 & 0x3) + TEXTURE_RES * 19 / 16) { // grass edge shadow
                                    gsd_tempA = gsd_tempA * 2 / 3;
                                }
                                break;
                            case static_cast<int>(BlockId::BLOCK_BRICKS):
                                textureAtlas[x + y * TEXTURE_RES + static_cast<int>(blockType) * (TEXTURE_RES * TEXTURE_RES) * 3] = Block::REDSTONE_BLOCK;
                                if ((x + y / 4 * 4) % 8 == 0 || y % 4 == 0) { // gap between bricks
                                    textureAtlas[x + y * TEXTURE_RES + static_cast<int>(blockType) * (TEXTURE_RES * TEXTURE_RES) * 3] = Block::DIORITE;
                                }
                                break;
                            case static_cast<int>(BlockId::BLOCK_LEAVES):
                                textureAtlas[x + y * TEXTURE_RES + static_cast<int>(blockType) * (TEXTURE_RES * TEXTURE_RES) * 3] = Block::EMERALD_BLOCK;
                                if (rand.nextInt(2) == 0) {
                                    textureAtlas[x + y * TEXTURE_RES + static_cast<int>(blockType) * (TEXTURE_RES * TEXTURE_RES) * 3] = Block::AIR;
                                }
                            default:
                                break;
                        }
                    }
                }

#ifndef HOSTED
                sleep();
#endif
            }
        }
    }

    void doPhysics() {

    }

    void runLoop() {
        long startTime = currentTimeMillis();

        while (true) {
            long time = currentTimeMillis();
            
            handleEvents();

            sinYaw = cameraYaw.sin();
            cosYaw = cameraYaw.cos();
            sinPitch = cameraPitch.sin();
            cosPitch = cameraPitch.cos();
            
            lightDirection.y = (Fix16(fix16_from_int(time)) / Fix16(10000.0)).sin();
            
            lightDirection.x = 0.0; //lightDirection.y * 0.5f;
            lightDirection.z = (Fix16(fix16_from_int(time)) / Fix16(10000.0)).cos();
            
            
            if(lightDirection.y < 0.0)
            {
                Vec3::lerp(SC_TWILIGHT, SC_DAY, Fix16(0) - lightDirection.y, sunColor);
                Vec3::lerp(AC_TWILIGHT, AC_DAY, Fix16(0) - lightDirection.y, ambColor);
                Vec3::lerp(YC_TWILIGHT, YC_DAY, Fix16(0) - lightDirection.y, skyColor);
            } else {
                Vec3::lerp(SC_TWILIGHT, SC_NIGHT, lightDirection.y, sunColor);
                Vec3::lerp(AC_TWILIGHT, AC_NIGHT, lightDirection.y, ambColor);
                Vec3::lerp(YC_TWILIGHT, YC_NIGHT, lightDirection.y, skyColor);
            }

            int physStartTime = currentTimeMillis();
            
            while (physStartTime - startTime > 10L) {
                // adjust camera
                cameraYaw += Fix16((int16_t)mouseDelta.x) / 400.0F;
                cameraPitch -= Fix16((int16_t)mouseDelta.y) / 400.0F;

                if (cameraPitch < -1.57F)
                    cameraPitch = -1.57F;

                if (cameraPitch > 1.57F)
                    cameraPitch = 1.57F;
                
                startTime += 10L;
                Fix16 inputX = Fix16(fix16_from_int((input.contains(KeyEvent::VK_D) - input.contains(KeyEvent::VK_A)))) * Fix16(0.02F);
                Fix16 inputZ = Fix16(fix16_from_int((input.contains(KeyEvent::VK_W) - input.contains(KeyEvent::VK_S)))) * Fix16(0.02F);
                velocityX *= 0.5F;
                velocityY *= 0.99F;
                velocityZ *= 0.5F;
                velocityX += sinYaw * inputZ + cosYaw * inputX;
                velocityZ += cosYaw * inputZ - sinYaw * inputX;
                velocityY += 0.003F;
                
                
                //check for movement on each axis individually (thanks JuPaHe64!)

                bool skipOne = false;
                bool stopAll = false;

                Axis axes[] = { Axis::Z, Axis::X, Axis::Y };
                for (Axis axis : axes) {
                    Fix16 newPlayerX = playerX + velocityX * (int16_t)(axis == Axis::X);
                    Fix16 newPlayerY = playerY + velocityY * (int16_t)(axis == Axis::Y);
                    Fix16 newPlayerZ = playerZ + velocityZ * (int16_t)(axis == Axis::Z);
                    
                    for (int colliderIndex = 0; colliderIndex < 12 && !stopAll; colliderIndex++) {
                        Fix16 xOff = (int16_t)  (colliderIndex       & 1);
                        Fix16 yOff = (int16_t) ((colliderIndex >> 2) - 1);
                        Fix16 zOff = (int16_t)  (colliderIndex >> 1  & 1);
                        // magic
                        int colliderBlockX = (int16_t)(newPlayerX + xOff * 0.6F - 0.3F ) - WORLD_SIZE;
                        int colliderBlockY = (int16_t)(newPlayerY + yOff * 0.8F + 0.65F) - WORLD_HEIGHT;
                        int colliderBlockZ = (int16_t)(newPlayerZ + zOff * 0.6F - 0.3F ) - WORLD_SIZE;

                        if(colliderBlockY < 0)
                            continue;

                        // check collision with world bounds and world blocks
                        if (colliderBlockX < 0 || colliderBlockZ < 0
                                || colliderBlockX >= WORLD_SIZE || colliderBlockY >= WORLD_HEIGHT || colliderBlockZ >= WORLD_SIZE
                                || world[colliderBlockX][colliderBlockY][colliderBlockZ] != BlockId::BLOCK_AIR) {
                            
                            if (axis != Axis::Y) { // not checking for vertical movement
                                skipOne = true;
                                break;
                            }
                                //continue OUTER; // movement is invalid
                            
                            // if we're falling, colliding, and we press space
                            if (input.contains(KeyEvent::VK_SPACE) == true && velocityY > 0.0F) {
                                velocityY = -0.1F; // jump
                                stopAll = true;
                                break;
                                //break OUTER;
                            }
                            
                            // stop vertical movement
                            velocityY = 0.0F;
                            stopAll = true;
                            break;
                            //break OUTER;
                        }
                    }

                    if (stopAll) {
                        break;
                    }

                    if (skipOne) {
                        skipOne = false;
                        continue;
                    }
                    
                    playerX = newPlayerX;
                    playerY = newPlayerY;
                    playerZ = newPlayerZ;
                }

#ifndef HOSTED
                sleep();
#endif
            }
            
            if(hoveredBlockPosX > -1) { // all axes will be -1 if nothing hovered
                // break block
                if (input.contains(KeyEvent::MOUSE_LEFT) == true) {
                    world[hoveredBlockPosX][hoveredBlockPosY][hoveredBlockPosZ] = BlockId::BLOCK_AIR;
                    input.erase(KeyEvent::MOUSE_LEFT);
                }
                
                
                if(placeBlockPosY > 0) {
                    // place block
                    if (input.contains(KeyEvent::MOUSE_RIGHT)) {
                        world[placeBlockPosX][placeBlockPosY][placeBlockPosZ] = hotbar[heldBlockIndex];
                        input.erase(KeyEvent::MOUSE_RIGHT);
                    }
                }
            }
            
            for (int colliderIndex = 0; colliderIndex < 12; colliderIndex++) {
                Fix16 xOff = (int16_t)( colliderIndex       & 1);
                Fix16 yOff = (int16_t)((colliderIndex >> 2) - 1);
                Fix16 zOff = (int16_t)( colliderIndex >> 1  & 1);

                int magicX = (int)(playerX + xOff * 0.6F - 0.3F ) - WORLD_SIZE;
                int magicY = (int)(playerY + yOff * 0.8F + 0.65F) - WORLD_HEIGHT;
                int magicZ = (int)(playerZ + zOff * 0.6F - 0.3F ) - WORLD_SIZE;
                
                // check if hovered block is within world boundaries
                if (magicX >= 0 && magicY >= 0 && magicZ >= 0 && magicX < WORLD_SIZE && magicY < WORLD_HEIGHT && magicZ < WORLD_SIZE)
                    world[magicX][magicY][magicZ] = BlockId::BLOCK_AIR;
            }

            // render the screen
            newHoverBlockPosX = -1;
            newHoverBlockPosY = -1;
            newHoverBlockPosZ = -1;
            
            render();
            
            hoveredBlockPosX = newHoverBlockPosX;
            hoveredBlockPosY = newHoverBlockPosY;
            hoveredBlockPosZ = newHoverBlockPosZ;
            
            placeBlockPosX += hoveredBlockPosX;
            placeBlockPosY += hoveredBlockPosY;
            placeBlockPosZ += hoveredBlockPosZ;
            
            #ifdef HOSTED
            deltaTime = currentTimeMillis() - time;
            
            // reset mouse delta so if we stop moving the mouse it doesn't drift
            if(currentTimeMillis() - lastMouseMove > 25)
                mouseDelta = Point();
            
            if(deltaTime < 14)
                sleep_ms(16 - deltaTime);
            #endif
            
            paint(g2d);
            
            if(hovered) {
                mouseLocation = getMouseLocation();
                
                mouseDelta = Point(mouseLocation.x - lastMouseLocation.x, mouseLocation.y - lastMouseLocation.y);
                
                recenterMouse(frame);
                
                mouseLocation = getMouseLocation();
                lastMouseLocation = mouseLocation;
            } else {
                mouseDelta = Point();
            }
        }
    }
    
private:
    void recenterMouse(JFrame frame) {}
    
public:
    void paint(Graphics2D g2d)
    {
        g2d.clear();

        /*g2d.drawImage(crosshair, WINDOW_WIDTH / 2 - CROSS_SIZE / 2, WINDOW_HEIGHT / 2 - CROSS_SIZE / 2);
        
        g2d.setColor(Color::gray().darker());
        g2d.setStroke(BasicStroke(4));
        
        const int hotbarItemSize = 64;
        const int padding = 2;
        const int hotbarX = WINDOW_WIDTH / 2 - (hotbarItemSize * HOTBAR_LEN) / 2;
        const int hotbarY = WINDOW_HEIGHT - hotbarItemSize - 10;
        
        // draw transparent background
        g2d.setColor(Color(0x33, 0x33, 0x33, 0x7F).darker());
        g2d.fillRect(hotbarX - 2, hotbarY - 2, hotbarItemSize * HOTBAR_LEN + 4, hotbarItemSize + 4);
        
        g2d.setColor(Color::gray().darker());
        g2d.drawRect(hotbarX - 2, hotbarY - 2, hotbarItemSize * HOTBAR_LEN + 4, hotbarItemSize + 4);
        
        for(int i = 0; i < HOTBAR_LEN; i++)
        {
            g2d.drawImage(textureAtlasImage,
                    hotbarX + i * hotbarItemSize + padding, hotbarY + padding, //draw bounds min
                    hotbarX + i * hotbarItemSize + hotbarItemSize - padding, hotbarY + hotbarItemSize - padding, //draw bounds max
                    0, TEXTURE_RES * (static_cast<int>(hotbar[i]) * 3 + 1), // sample bounds min
                    TEXTURE_RES, TEXTURE_RES * (static_cast<int>(hotbar[i]) * 3 + 2) // sample bounds max
                	);
        }
        
        g2d.setColor(Color::white());
        g2d.setStroke(BasicStroke(5));
        
        g2d.drawRect(hotbarX + heldBlockIndex * hotbarItemSize, hotbarY, hotbarItemSize, hotbarItemSize);
            
        if(deltaTime > 16 || deltaTime == 0) // 16ms = 60fps
            g2d.setColor(Color::red());
        else
            g2d.setColor(Color::white());
        
        g2d.setFont(font);
        
        std::string str;
        if(deltaTime == 0)
            str = "Houston, we have a problem!";
        else
            str = "" + std::to_string(1000 / deltaTime) + " fps, 0 chunk updates";
        
        g2d.drawString(str, 0, 10);

        std::string x = std::string("X: "); x += std::to_string((float)(playerX) - 64); x += " ; "; x += std::to_string((float)velocityX);
        std::string y = std::string("Y: "); y += std::to_string((float)(playerY) - 64); y += " ; "; y += std::to_string((float)velocityY);
        std::string z = std::string("Z: "); z += std::to_string((float)(playerZ) - 64); z += " ; "; z += std::to_string((float)velocityZ);

        g2d.drawString(x, 0, 20);
        g2d.drawString(y, 0, 30);
        g2d.drawString(z, 0, 40);*/

        repaint();
    }
    
private:
    // ew java
    static int integer(bool b)
    {
        return b ? 1 : 0;
    }
};

int abs(int x) {
    if (x > 0) {
        return x;
    } else {
        return -x;
    }
}

int main() {
	memset(world, 0, sizeof(world));

    print(1234);

	frame = JFrame();

	Minecraft4k m4k = {};

	/*for(int x = 0; x < CROSS_SIZE; x++)
	{
		for(int y = 0; y < CROSS_SIZE; y++)
		{
		if(((abs(x - CROSS_SIZE / 2) + 2) * (abs(y - CROSS_SIZE / 2) + 2) < 1 + isqrt((CROSS_SIZE * CROSS_SIZE / 2)))
			&& (abs(x - CROSS_SIZE / 2) + 2) + (abs(y - CROSS_SIZE / 2) + 2) < CROSS_SIZE * 0.4375f
			&& (abs(x - CROSS_SIZE / 2) + 2) + (abs(y - CROSS_SIZE / 2) + 2) > CROSS_SIZE * 0.296875f)
			crosshair.setRGB(x, y, 0xFFFFFFFF);
		}
	}*/

	m4k.run();
}

#ifndef HOSTED
    extern "C" void _start() {
        main();
    }
#endif