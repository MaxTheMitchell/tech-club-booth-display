//#define PxMATRIX_double_buffer true
#include <PxMatrix.h>
#include <functional>
using namespace std;
  
#define P_LAT 22
#define P_A 19
#define P_B 23
#define P_C 18
#define P_D 5
#define P_E 15
#define P_OE 16
hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

#define MAT_SIZE 64
#define BLACK 0
#define WHITE 65535

// This defines the 'on' time of the display is us. The larger this number,
// the brighter the display. If too large the ESP will crash
uint8_t display_draw_time=10; //30-70 is usually fine
const uint16_t LOGO[64*64] = {55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 57384, 57384, 57384, 57384, 57384, 57384, 57384, 57384, 57384, 57384, 55336, 57384, 57384, 57384, 57384, 57384, 57384, 57384, 57384, 55336, 55336, 55336, 55336, 57384, 57384, 57384, 55336, 57384, 57384, 55336, 55336, 57384, 57384, 57384, 55336, 55336, 55336, 55336, 57384, 57384, 57384, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 57384, 40998, 26628, 26628, 26628, 26628, 26628, 26628, 26628, 26628, 34853, 57384, 36901, 26628, 26628, 26628, 26628, 26628, 26628, 30724, 55336, 55336, 55336, 57384, 45095, 24612, 14338, 12290, 18467, 30756, 49191, 57384, 38918, 26628, 28676, 53288, 55336, 55336, 55336, 30756, 26628, 36901, 57384, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 57384, 26628, 0, 0, 0, 0, 0, 0, 0, 0, 10274, 59433, 18434, 0, 0, 0, 0, 0, 0, 0, 55336, 55336, 57384, 28708, 0, 0, 0, 0, 0, 0, 26660, 59433, 22531, 0, 0, 53288, 55336, 55336, 53288, 0, 0, 18467, 57384, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 57384, 40998, 22531, 22531, 18435, 0, 0, 14338, 22531, 22531, 30757, 57384, 18435, 0, 0, 22531, 22531, 22531, 22531, 28708, 55336, 57384, 32805, 0, 0, 0, 24612, 28676, 22563, 8193, 43046, 59433, 24579, 0, 0, 53288, 55336, 55336, 53288, 0, 0, 18467, 57384, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 57384, 57384, 59433, 47143, 0, 0, 36869, 59433, 57384, 57384, 57384, 18435, 0, 10241, 59433, 59433, 59433, 59433, 57384, 55336, 53288, 0, 0, 0, 49191, 57384, 57384, 57384, 53288, 55336, 57384, 24579, 0, 0, 55336, 57384, 57384, 55336, 8193, 0, 18467, 57384, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 57384, 47143, 0, 0, 34821, 57384, 55336, 55336, 57384, 18435, 0, 0, 40998, 40998, 40998, 40998, 51240, 57384, 40998, 0, 0, 32805, 59433, 55336, 55336, 55336, 55336, 55336, 57384, 24579, 0, 0, 36869, 38918, 38917, 36901, 0, 0, 18467, 57384, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 57384, 47143, 0, 0, 34821, 57384, 55336, 55336, 57384, 18435, 0, 0, 0, 0, 0, 0, 38950, 59433, 34853, 0, 0, 43046, 57384, 55336, 55336, 55336, 55336, 55336, 57384, 24579, 0, 0, 0, 0, 0, 0, 0, 0, 18467, 57384, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 57384, 47143, 0, 0, 34821, 57384, 55336, 55336, 57384, 18435, 0, 0, 10241, 10241, 10241, 10241, 43046, 59433, 32805, 0, 0, 43046, 57384, 55336, 55336, 55336, 55336, 55336, 57384, 24579, 0, 0, 0, 0, 0, 0, 0, 0, 18467, 57384, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 57384, 47143, 0, 0, 34821, 57384, 55336, 55336, 57384, 18435, 0, 10241, 55336, 55336, 55336, 55336, 55336, 57384, 38950, 0, 0, 32805, 57384, 55336, 55336, 55336, 55336, 55336, 57384, 24579, 0, 0, 51239, 55336, 55336, 53288, 0, 0, 18467, 57384, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 57384, 47143, 0, 0, 34821, 57384, 55336, 55336, 57384, 18435, 0, 10241, 57384, 57384, 57384, 57384, 57384, 57384, 49191, 0, 0, 8193, 51239, 57384, 57384, 57384, 55336, 55336, 57384, 24579, 0, 0, 53288, 55336, 55336, 55336, 0, 0, 18467, 57384, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 57384, 47143, 0, 0, 34821, 57384, 55336, 55336, 57384, 18435, 0, 0, 24579, 24579, 24579, 24579, 24579, 47143, 57384, 24611, 0, 0, 0, 26660, 30724, 26660, 12290, 36901, 59433, 24579, 0, 0, 53288, 55336, 55336, 53288, 0, 0, 18467, 57384, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 57384, 45095, 0, 0, 34821, 57384, 55336, 55336, 57384, 18434, 0, 0, 0, 0, 0, 0, 0, 40998, 57384, 53288, 18466, 0, 0, 0, 0, 0, 0, 18466, 59433, 22531, 0, 0, 53288, 55336, 55336, 53288, 0, 0, 18467, 57384, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 51239, 24580, 24579, 45094, 57384, 55336, 55336, 57384, 34853, 24579, 24579, 24579, 24579, 24579, 24579, 24579, 47143, 55336, 55336, 55336, 36901, 20515, 12290, 12289, 16386, 26660, 45095, 57384, 38917, 24579, 28676, 53288, 55336, 55336, 55336, 28708, 24579, 34853, 57384, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 57384, 57384, 55336, 55336, 55336, 55336, 55336, 57384, 57384, 57384, 57384, 57384, 57384, 57384, 57384, 55304, 55304, 55304, 55304, 57352, 57384, 55336, 55336, 57384, 57384, 57384, 55336, 57384, 57384, 57384, 55336, 55336, 55336, 55336, 57384, 57384, 57384, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55304, 57417, 55692, 49677, 49709, 53709, 57546, 55304, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55304, 57611, 41742, 31695, 35985, 31792, 31727, 35694, 51789, 57449, 57384, 57352, 55304, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55304, 57643, 35630, 40244, 61277, 65535, 52857, 40179, 33873, 29614, 37678, 41677, 43758, 57675, 55304, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55304, 57675, 33581, 42357, 65535, 52825, 42292, 63390, 65535, 46486, 54938, 44405, 44438, 38099, 33678, 53838, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55303, 57708, 35694, 44470, 65535, 65535, 65535, 59164, 40179, 65535, 65535, 65535, 54938, 63422, 59164, 44502, 37710, 57449, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55304, 57708, 27275, 25453, 50744, 65535, 65535, 65535, 65535, 38066, 65503, 65535, 65535, 52857, 42292, 50712, 31760, 53773, 57351, 55304, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55304, 57740, 33614, 40211, 42260, 25388, 65503, 65535, 65535, 63390, 25388, 59164, 65535, 61309, 54970, 25388, 44373, 35856, 49935, 47758, 55692, 55304, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55304, 57740, 31565, 46583, 57083, 42292, 33808, 52825, 54970, 54970, 50712, 33840, 21130, 48599, 44405, 31695, 25356, 38034, 35921, 38131, 38131, 31662, 51854, 57352, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55304, 57514, 35597, 42357, 65535, 38066, 61309, 29614, 42260, 42260, 40179, 42292, 46518, 40179, 40147, 42260, 40147, 40179, 40179, 38034, 12678, 16904, 42324, 31662, 55724, 55304, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55304, 53806, 12743, 63390, 52825, 57083, 57051, 40179, 23243, 10597, 12678, 59196, 46518, 12678, 10597, 10597, 10597, 10597, 10565, 0, 0, 0, 12710, 40244, 43758, 57384, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 57384, 43790, 23307, 44373, 65503, 65535, 31727, 50712, 0, 0, 27469, 57051, 0, 0, 0, 0, 0, 0, 12710, 0, 0, 0, 0, 40147, 35727, 57578, 55304, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 57481, 37710, 40179, 0, 29582, 21162, 0, 50744, 0, 0, 57083, 12710, 0, 12710, 0, 8484, 12710, 0, 38066, 16936, 0, 16936, 0, 29582, 33840, 55692, 55304, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55304, 57578, 35791, 33840, 0, 16936, 42260, 0, 44405, 0, 40147, 27501, 0, 14791, 31727, 0, 23275, 31727, 0, 0, 0, 0, 40179, 0, 21130, 36018, 51693, 55304, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55304, 57643, 33743, 50776, 16936, 0, 31695, 23275, 38034, 21162, 35921, 0, 10565, 29614, 46518, 0, 0, 0, 0, 44373, 23243, 0, 0, 0, 16904, 38131, 49709, 55304, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55304, 55627, 33776, 46518, 42292, 42260, 23243, 54970, 63422, 59196, 23243, 40147, 42292, 42260, 57083, 0, 33840, 52825, 48599, 65535, 63390, 42260, 52857, 8452, 12710, 40211, 49741, 55304, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55304, 57611, 35824, 33840, 0, 0, 31695, 46486, 40147, 44373, 38034, 8484, 0, 44405, 61277, 25356, 65535, 65535, 65503, 65503, 65535, 65535, 63422, 54938, 27501, 38098, 49677, 55304, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55304, 57546, 33646, 57083, 48631, 31695, 31695, 46486, 50744, 46486, 31727, 31695, 27469, 35953, 54970, 25388, 59196, 50712, 35953, 48631, 35921, 52857, 27501, 61309, 65503, 33872, 53676, 55304, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 57417, 39694, 48631, 46518, 48599, 48599, 44405, 48631, 44405, 46518, 48599, 29582, 46486, 61309, 33840, 65535, 63390, 61277, 63422, 57083, 63390, 57051, 65535, 65535, 31663, 55595, 55304, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 57352, 47758, 40244, 48631, 52825, 52857, 40179, 31695, 40147, 50712, 50712, 27469, 57051, 65535, 27501, 48631, 57051, 44405, 27501, 33840, 54938, 59196, 44405, 48696, 35630, 57449, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55304, 55692, 33808, 29647, 0, 0, 0, 0, 35953, 44373, 42260, 16904, 50744, 63422, 19049, 33840, 38066, 40147, 10597, 0, 0, 0, 14823, 38131, 47725, 57352, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 57449, 39758, 42324, 0, 0, 0, 8484, 38034, 46518, 46518, 33840, 48631, 63422, 29582, 48599, 46486, 44373, 21130, 0, 0, 0, 40147, 31663, 55627, 55304, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55304, 53773, 33840, 33808, 0, 0, 0, 38034, 52857, 54970, 35953, 59196, 65535, 35921, 57051, 52857, 46518, 19017, 0, 0, 16936, 40211, 43693, 57384, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 57417, 41774, 40212, 14823, 0, 8452, 38034, 48631, 50712, 29614, 52857, 50744, 35953, 46486, 48631, 46486, 19049, 0, 0, 44405, 31565, 57643, 55304, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55304, 57708, 31630, 42325, 0, 0, 38034, 44405, 52825, 27501, 31727, 31727, 27469, 46486, 46518, 44373, 19049, 0, 38066, 31759, 49741, 57352, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55304, 53838, 31727, 40211, 8484, 29614, 52857, 52857, 42260, 31727, 35953, 38034, 52825, 54970, 42260, 12678, 31695, 36018, 41678, 57417, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 57352, 51886, 31792, 40179, 12710, 19049, 21130, 12678, 19017, 35921, 0, 19049, 21162, 14791, 31727, 38131, 37613, 57514, 55304, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 57384, 49871, 31760, 42260, 0, 0, 0, 19017, 38066, 0, 0, 0, 33808, 38098, 37646, 57546, 55304, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55304, 57384, 51886, 31727, 44405, 16936, 0, 0, 12710, 0, 0, 40179, 33905, 39662, 57546, 55304, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 57352, 53805, 33678, 40244, 31727, 0, 0, 21162, 42325, 29614, 45742, 57481, 55304, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55304, 57708, 39726, 33905, 42292, 40179, 38098, 31565, 53805, 57384, 55304, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55304, 57449, 51822, 33711, 29614, 43726, 57611, 55304, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55304, 55304, 57643, 55757, 57384, 55304, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 57384, 57384, 53288, 53288, 55336, 57384, 57384, 55336, 57384, 57384, 57384, 55336, 55336, 55336, 55336, 55336, 55304, 57352, 57384, 55336, 55336, 55336, 55336, 55336, 57384, 57384, 55336, 55336, 57384, 57384, 57384, 55336, 55336, 57384, 57384, 57384, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 36901, 16418, 0, 0, 10241, 22563, 45095, 57384, 32773, 16386, 22531, 55336, 55336, 55336, 55336, 55336, 53288, 22531, 16386, 34853, 57384, 55336, 57384, 40998, 16386, 16386, 49191, 57384, 40998, 18467, 14338, 10241, 12290, 16386, 26660, 47143, 57384, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 53288, 18467, 0, 0, 0, 0, 0, 0, 30757, 59433, 20483, 0, 0, 53288, 55336, 55336, 55336, 55336, 53288, 0, 0, 24579, 57384, 55336, 57384, 34853, 0, 0, 45095, 59433, 28676, 0, 0, 0, 0, 0, 0, 0, 43046, 57384, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 57384, 26660, 0, 0, 14370, 34853, 38950, 32805, 16418, 49191, 59433, 22531, 0, 0, 53288, 55336, 55336, 55336, 55336, 53288, 0, 0, 24580, 57384, 55336, 57384, 34853, 0, 0, 47143, 59433, 30724, 0, 0, 38950, 43046, 34853, 0, 0, 18467, 57384, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 49191, 0, 0, 14370, 55336, 57384, 57384, 57384, 57384, 55336, 57384, 22531, 0, 0, 53288, 55336, 55336, 55336, 55336, 53288, 0, 0, 24580, 57384, 55336, 57384, 34853, 0, 0, 47143, 59433, 30724, 0, 0, 55336, 59433, 61481, 18467, 0, 14338, 57384, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 57384, 36901, 0, 0, 38950, 57384, 55336, 55336, 55336, 55336, 55336, 57384, 22531, 0, 0, 53288, 55336, 55336, 55336, 55336, 53288, 0, 0, 24580, 57384, 55336, 57384, 34853, 0, 0, 47143, 59433, 30724, 0, 0, 30724, 32805, 24611, 0, 0, 36901, 57384, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 57384, 32773, 0, 0, 45095, 57384, 55336, 55336, 55336, 55336, 55336, 57384, 22531, 0, 0, 53288, 55336, 55336, 55336, 55336, 53288, 0, 0, 24580, 57384, 55336, 57384, 34853, 0, 0, 47143, 59433, 30724, 0, 0, 0, 0, 0, 0, 8193, 47143, 57384, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 57384, 32773, 0, 0, 45094, 57384, 55336, 55336, 55336, 55336, 55336, 57384, 22531, 0, 0, 53288, 55336, 55336, 55336, 55336, 53288, 0, 0, 24580, 57384, 55336, 57384, 34853, 0, 0, 47143, 59433, 30724, 0, 0, 24579, 26628, 22563, 0, 0, 0, 51240, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 57384, 38950, 0, 0, 32805, 59433, 55336, 55336, 55336, 55336, 55336, 57384, 22531, 0, 0, 53288, 55336, 55336, 55336, 55336, 53288, 0, 0, 22531, 57384, 55336, 57384, 34853, 0, 0, 47143, 59433, 30724, 0, 0, 53288, 59433, 59433, 38950, 0, 0, 40998, 57384, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 51239, 0, 0, 0, 47143, 57384, 57384, 57384, 51240, 53288, 57384, 22531, 0, 0, 53288, 55336, 55336, 55336, 55336, 55336, 12290, 0, 8193, 53288, 59433, 57384, 18467, 0, 0, 51239, 59433, 30724, 0, 0, 53288, 59433, 59433, 34853, 0, 0, 40998, 57384, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 57384, 28708, 0, 0, 0, 18434, 22531, 16386, 0, 34853, 59433, 22531, 0, 0, 10241, 12289, 12289, 12289, 20515, 57384, 30757, 0, 0, 12290, 24612, 14338, 0, 0, 20515, 57384, 57384, 30724, 0, 0, 22531, 24580, 18435, 0, 0, 0, 51239, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 24612, 0, 0, 0, 0, 0, 0, 22563, 59433, 20483, 0, 0, 0, 0, 0, 0, 8225, 55336, 55336, 20515, 0, 0, 0, 0, 0, 12290, 51239, 55336, 57384, 30724, 0, 0, 0, 0, 0, 0, 8193, 40998, 57384, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 57384, 45095, 30756, 22531, 20515, 26660, 38950, 51240, 57384, 43046, 34853, 34853, 34853, 34853, 34853, 34853, 38950, 55336, 55336, 55336, 40998, 26660, 20515, 24611, 36901, 53288, 57384, 55336, 55336, 49191, 36901, 30756, 28708, 26660, 32805, 40998, 53288, 57384, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 57384, 57384, 57384, 57384, 57384, 57384, 55336, 55336, 57384, 57384, 57384, 57384, 57384, 57384, 57384, 57384, 55336, 55336, 55336, 57384, 57384, 57384, 57384, 57384, 55336, 55336, 55336, 55336, 55336, 57384, 57384, 57384, 57384, 57384, 57384, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336, 55336};
int grid[64][64];
int newGrid[64][64]; // globaly scoped to save memory

PxMATRIX display(64,64,P_LAT, P_OE,P_A,P_B,P_C,P_D,P_E);

void IRAM_ATTR display_updater(){
  // Increment the counter and set the time of ISR
  portENTER_CRITICAL_ISR(&timerMux);
  display.display(display_draw_time);
  portEXIT_CRITICAL_ISR(&timerMux);
}

void display_update_enable(bool is_enable){
  if (is_enable){
    timer = timerBegin(0, 80, true);
    timerAttachInterrupt(timer, &display_updater, true);
    timerAlarmWrite(timer, 4000, true);
    timerAlarmEnable(timer);
  }else{
    timerDetachInterrupt(timer);
    timerAlarmDisable(timer);
  }
}

void setup() {
  Serial.begin(9600);
  display.begin(32);
  display.clearDisplay();
  display_update_enable(true);
  delay(4000);
}

const int ANIMATION_COUNT = 6;
const function<void()> ANIMATIONS[ANIMATION_COUNT] = { &tree,  &wolfFrame, &toothpaste, &gameOfLife, &rockPaperSis, &curve };
void loop() {
  times(ANIMATION_COUNT, [](int a) {
    logo();
    ANIMATIONS[a]();
  });
}

void logo(){
  for(int x=0; x<MAT_SIZE; x++){
    for(int y=0; y<MAT_SIZE; y++){
      if(y % 2 == 0)
        display.drawPixel(x, y, LOGO[y*64+x]);
      else 
        display.drawPixel(63 - x, y, LOGO[y*64 + 63 - x]); 
    }
    delay(10);
   }
    delay(1000);
   times(MAT_SIZE, [](int i){
    int x1 = i;
    int y1 = i;
    int x2 = 63 - i;
    int y2 = 63 - i;
    display.drawLine(x1,y1,x1,y2,BLACK);
    display.drawLine(x1,y1,x2,y1,BLACK);
    display.drawLine(x1,y2,x2,y2,BLACK);
    display.drawLine(x2,y1,x2,y2,BLACK);
    delay(10);
   });
}



void tree(){
  branch(30, 63, 32, 0.5 * PI, 0);
  delay(500);
  branch(14, 63, 16, 0.5 * PI, 0);
  delay(500);
  branch(45, 63, 16, 0.5 * PI, 0);
  delay(500);
  branch(6, 63, 9, 0.5 * PI, 0);
  delay(500);
  branch(52, 63, 9, 0.5 * PI, 0);
  delay(500);
  branch(23, 63, 9, 0.5 * PI, 0);
  delay(500);
  branch(38 , 63, 9, 0.5 * PI, 0);
  delay(1000);
  
}

#define ROTATION (PI/5)
#define LEN_CUT 0.5
void branch(float x, float y, int len, float rad, int darkness){
  float point[2] = {len, 0};
  if(len > 1){
    rotatePoint(point, rad);
    display.drawLine(x, y, x + point[0], y + point[1], display.color565(0 , darkness, 255));
    delay(7);
    for(int i=-2; i<3; i++)
      branch(x + point[0], y + point[1], len * LEN_CUT, rad + ROTATION*i, darkness + 15);
  }
}

void curve(){
  const int SIZE = 100;
  float point[2];
  for (int CURVES = 2; CURVES< 10; CURVES*=2){
  display.clearDisplay();
  for(int i=0; i<SIZE; i++){
    float piSlice = PI * 2 * i / (float)SIZE;
    point[0] = 31 * sin(piSlice);
    point[1] = 31 * sin(piSlice) * cos(piSlice);
    for(int c=0; c<CURVES; c++){
      display.drawPixel((int)point[0] + 31,(int)point[1] + 31, WHITE);
      rotatePoint(point,PI/CURVES); 
    }
    delay(10 + CURVES );
  }
  }
}

void toothpaste(){
  const int SIZE = 100;
  const int CURVES = 20;
  float point[2];
  for(int i=0; i<SIZE; i++){
    float piSlice = PI * 2 * i / (float)SIZE;
    point[0] = 31 * sin(piSlice);
    point[1] = 31 * sin(piSlice) * cos(piSlice);
    for(int c=0; c<CURVES; c++){
      display.drawPixel((int)point[0] + 31,(int)point[1] + 31, display.color565(255, 255, 255 * c / CURVES));
      rotatePoint(point, (PI/4)/CURVES);
    }
    delay(15);
  }
  delay(500);
  for(int i=0; i<SIZE; i++){
    float piSlice = PI * 2 * i / (float)SIZE;
    point[0] = 31 * sin(piSlice);
    point[1] = 31 * sin(piSlice) * cos(piSlice);
    rotatePoint(point, PI/2);
    for(int c=0; c<CURVES; c++){
      display.drawPixel((int)point[0] + 31,(int)point[1] + 31, display.color565(255 * c / CURVES, 255, 255));
      rotatePoint(point, (PI/4)/CURVES);
    }
    delay(15);
  }
  delay(500);
}

float *rotatePoint(float point[2], float rad){
    float tmp = point[0] * cos(rad) + point[1] * sin(rad);
    point[1] = point[1] * cos(rad) - point[0] * sin(rad);
    point[0] = tmp;
    return point;
}

void gameOfLife(){
  mapGrid(grid, [](int x, int y) -> int { return ((x + y) % 2 == 0); });
  for(int i = 0; i<130 ; i++){
     forEachGrid(grid, [&](int val, int x, int y){ display.drawPixel(x, y, grid[x][y] ? WHITE : BLACK); });
     playRound(grid);
     delay(50);
  }
}
 
void playRound(int grid[64][64]){
  copyToNewGrid();
  mapGrid(grid, [&](int x, int y) -> int { return surviveGOL(x,y,newGrid); });
}

int surviveGOL(int x, int y, int grid[64][64]){
  int adj = adjs(x, y, grid, [](int val) -> bool{ return val; });
  return (adj == 3 || (grid[x][y] && adj == 4));
}

int adjs(int x, int y, int grid[64][64], function<bool(int)> func){
  int count = 0;
  for(int xx = -1; xx < 2; xx++)
    for(int yy = -1; yy < 2; yy++)
      if(inRange(xx+x) && inRange(yy+y) && func(grid[xx+x][yy+y]))
        count++;
  return count;
}

bool inRange(int val){
  return val >= 0 && val < 64;
}

void rockPaperSis(){
  mapGrid(grid, []() -> int {return (int)random(3); });
  times(200, [](){
    forEachGrid(grid, [](int val, int x, int y){
      switch(val){
        case 0:
          display.drawPixel(x, y, display.color565(255,0,0));
          break;
        case 1:
          display.drawPixel(x, y, display.color565(0,255, 0));
          break;
        case 2:
          display.drawPixel(x, y, display.color565(0,0, 255));
          break;
      }
    });
    copyToNewGrid();
    mapGrid(grid, [](int val, int x, int y) -> int{
       if(2 < adjs(x, y, grid, [&](int a) -> bool{ return (a == (val + 1) % 3); }))
        return (val + 1) % 3;
       return val;
    });
    delay(10);
  });
}

void wolfFrame(){
  const int RULE_COUNT = 3;
  const unsigned int RULES[RULE_COUNT] = {110, 30, 90};
  
  for(int s=0; s<3; s++){
    for(int r=0; r<RULE_COUNT; r++){
      if(s == 2){
        mapGrid(grid, [](int x, int y) -> int{ return (y == 0 && x % 3 == 0); });
        r=2;
      }else if(s)
        mapGrid(grid, [](int x, int y) -> int{ return (y == 0); });
      else 
        mapGrid(grid, [](int x, int y) -> int{ return (x==30 && y ==0); });
      for(int y=1; y<64; y++){
        for(int x=0; x<64; x++){
          unsigned int adjs =0;
          for(int xx=-1; xx<2; xx++)
            if(inRange(x+xx) && grid[x+xx][y-1])
              adjs |= (1UL << (xx+1));
          grid[x][y] = (RULES[r] >> adjs) & 1U;
        }
         forEachGrid(grid, [&](int val, int x, int y){ display.drawPixel(x, y, val ? WHITE : BLACK); });
         delay(20);
      }
      forEachGrid(grid, [&](int val, int x, int y){ display.drawPixel(x, y, val ? WHITE : BLACK); });
      delay(100);
    }
  }
}

void copyToNewGrid(){
  mapGrid(newGrid, [&](int x, int y) -> int { return grid[x][y]; });
}

//void forEach(int len, int *a, function<void(int, int)> func){
//  for(int i=0; i<len; i++) func(a[i], i);
//}
//
//void forEach(int len, int *a, function<void(int)> func){
//  forEach(len, a, [&](int val, int i){ func(val) };
//}

void loopRange(int s, int e, function<void(int)>func){
  for(int i=e; i<s; i++) func(i);
}

void loopRange(int s, int e, function<void()>func){
  loopRange(s, e, [&](int i){ func(); });
}

void times(int len, function<void(int)> func){
  for(int i=0; i<len; i++) func(i);
//  loopRange(0, len, [&](int i){ func(i); });
}

void times(int len, function<void()> func){
  times(len, [&](int i) { func(); });
}

void forEachGrid(int grid[64][64], function<void(int, int, int)> func){
  for(int x=0; x<64; x++) for(int y=0; y<64; y++) func(grid[x][y],x,y);
}

void forEachGrid(int grid[64][64], function<void(int)> func){
  forEachGrid(grid, [&](int val, int x, int y) { func(val); });
}

void mapGrid(int grid[64][64], function<int(int, int, int)> func){
  forEachGrid(grid, [&](int val, int x, int y){ grid[x][y] = func(val, x, y); });
}

void mapGrid(int grid[64][64], function<int(int, int)> func){
  mapGrid(grid, [&](int val, int x, int y) { return func(x, y); });
}

void mapGrid(int grid[64][64], function<int(int)> func){
  mapGrid(grid, [&](int val, int x, int y) -> int{ return func(val); });
}

void mapGrid(int grid[64][64], function<int()> func){
  mapGrid(grid, [&](int x, int y) -> int{ return func(); });
}
