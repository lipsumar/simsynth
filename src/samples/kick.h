#pragma once
 
#include <Arduino.h>
#include "mozzi_pgmspace.h"
 
#define BD_NUM_CELLS 4096
#define BD_SAMPLERATE 16384
 
CONSTTABLE_STORAGE(int8_t) BD_DATA [] = {0, -1, 0, -1, 0, -1, 0, -1, 0, -1, 0,
0, 0, 0, -1, -3, -8, -14, -22, -31, -41, -35, -10, -2, -13, -21, -23, -20, -18,
-16, -13, -8, -3, 6, 16, 30, 45, 61, 77, 92, 89, 38, -8, -4, 24, 44, 85, 106,
100, 101, 96, 96, 93, 92, 89, 89, 85, 86, 73, 54, 68, 75, 70, 59, 49, 38, 26,
13, -3, -21, -40, -62, -83, -106, -125, -126, -123, -121, -119, -117, -115,
-114, -112, -110, -109, -107, -106, -104, -103, -101, -100, -98, -97, -96, -94,
-93, -91, -90, -88, -86, -85, -83, -82, -80, -78, -77, -75, -74, -72, -70, -57,
-38, -23, -7, 8, 24, 38, 52, 66, 80, 93, 105, 117, 125, 125, 122, 120, 118, 116,
114, 106, 104, 109, 108, 106, 104, 103, 99, 94, 87, 80, 72, 64, 56, 48, 38, 29,
19, 9, -1, -11, -20, -30, -39, -49, -58, -67, -76, -84, -92, -99, -106, -110,
-109, -107, -106, -98, -78, -73, -78, -81, -80, -77, -73, -69, -65, -59, -53,
-47, -41, -34, -27, -20, -12, -4, 4, 12, 20, 28, 36, 44, 52, 59, 67, 74, 81, 87,
94, 99, 105, 110, 114, 116, 114, 112, 110, 108, 106, 104, 102, 100, 98, 94, 89,
91, 90, 87, 82, 77, 72, 67, 61, 54, 48, 42, 35, 29, 22, 14, 7, 0, -8, -15, -22,
-29, -36, -43, -50, -57, -64, -70, -76, -82, -88, -94, -99, -104, -108, -111,
-114, -116, -117, -115, -113, -111, -109, -107, -104, -96, -91, -88, -87, -84,
-81, -77, -72, -68, -63, -58, -53, -48, -43, -37, -32, -26, -21, -15, -9, -3, 3,
8, 14, 20, 25, 31, 36, 41, 47, 51, 56, 61, 65, 70, 74, 78, 81, 85, 88, 91, 94,
96, 98, 99, 100, 101, 101, 101, 101, 101, 100, 99, 98, 96, 95, 92, 87, 78, 69,
65, 64, 61, 58, 54, 51, 47, 43, 38, 33, 29, 24, 18, 13, 8, 2, -4, -9, -15, -20,
-25, -30, -35, -40, -45, -50, -55, -59, -64, -68, -72, -76, -80, -83, -87, -90,
-93, -96, -98, -100, -102, -103, -104, -104, -104, -104, -104, -103, -102, -101,
-100, -98, -97, -95, -93, -91, -89, -85, -81, -74, -67, -62, -59, -57, -54, -51,
-47, -43, -39, -35, -31, -27, -22, -17, -13, -8, -3, 2, 6, 11, 16, 20, 25, 29,
34, 38, 42, 46, 50, 53, 57, 61, 64, 67, 70, 73, 76, 79, 81, 83, 85, 87, 89, 91,
92, 93, 94, 95, 95, 95, 95, 95, 94, 93, 92, 91, 90, 88, 87, 85, 83, 81, 79, 77,
75, 73, 71, 68, 66, 63, 61, 58, 54, 50, 45, 41, 38, 36, 33, 30, 27, 24, 21, 17,
13, 9, 6, 2, -2, -6, -11, -15, -19, -22, -26, -30, -34, -38, -41, -44, -48, -51,
-54, -57, -60, -63, -66, -68, -71, -73, -75, -77, -79, -80, -82, -83, -85, -86,
-87, -88, -89, -89, -90, -90, -90, -90, -90, -90, -89, -89, -88, -87, -85, -84,
-83, -81, -79, -78, -76, -74, -72, -70, -68, -66, -64, -62, -60, -57, -55, -52,
-48, -43, -39, -35, -32, -30, -27, -24, -20, -17, -14, -10, -7, -3, 0, 4, 8, 12,
15, 19, 23, 27, 30, 34, 37, 41, 44, 47, 50, 53, 56, 59, 62, 64, 67, 69, 71, 73,
75, 77, 78, 80, 81, 82, 83, 84, 85, 85, 86, 86, 86, 86, 85, 85, 84, 84, 83, 82,
80, 79, 78, 76, 74, 73, 71, 69, 67, 66, 64, 62, 60, 58, 56, 54, 52, 50, 48, 46,
44, 42, 40, 38, 36, 34, 32, 30, 29, 27, 25, 23, 21, 19, 17, 15, 13, 11, 8, 6, 3,
-1, -4, -7, -10, -13, -16, -18, -21, -23, -26, -29, -32, -34, -37, -40, -42,
-45, -48, -51, -53, -56, -58, -61, -63, -66, -68, -70, -72, -74, -76, -78, -80,
-81, -83, -84, -85, -87, -88, -88, -89, -90, -90, -91, -91, -91, -92, -91, -91,
-91, -90, -89, -88, -87, -86, -85, -83, -82, -80, -78, -77, -75, -73, -71, -69,
-67, -65, -63, -61, -59, -57, -55, -53, -51, -49, -48, -46, -44, -42, -40, -38,
-36, -34, -32, -30, -28, -26, -25, -23, -21, -19, -17, -15, -13, -11, -9, -7,
-6, -4, -1, 1, 4, 7, 9, 12, 15, 17, 20, 22, 24, 26, 28, 31, 33, 35, 38, 40, 42,
45, 47, 49, 51, 53, 55, 57, 59, 61, 63, 64, 66, 68, 69, 70, 72, 73, 74, 75, 76,
77, 77, 78, 78, 79, 79, 79, 80, 80, 80, 79, 79, 79, 78, 77, 77, 76, 75, 73, 72,
71, 70, 68, 67, 65, 63, 62, 60, 58, 56, 54, 52, 50, 48, 47, 45, 43, 41, 39, 38,
36, 34, 32, 31, 29, 27, 26, 24, 22, 21, 19, 17, 16, 14, 13, 11, 9, 8, 6, 5, 3,
2, 1, -1, -2, -3, -5, -6, -7, -9, -10, -11, -12, -13, -14, -16, -17, -18, -19,
-20, -21, -22, -23, -24, -25, -26, -27, -29, -30, -31, -33, -34, -35, -37, -38,
-39, -40, -41, -43, -44, -45, -46, -47, -48, -50, -51, -52, -53, -54, -55, -56,
-57, -58, -58, -59, -60, -60, -61, -62, -62, -63, -63, -64, -64, -64, -65, -65,
-65, -65, -65, -65, -64, -64, -64, -63, -62, -62, -61, -60, -59, -58, -57, -56,
-55, -53, -52, -51, -49, -48, -46, -45, -44, -42, -41, -39, -37, -35, -34, -32,
-30, -29, -27, -25, -24, -22, -21, -19, -17, -16, -14, -13, -11, -10, -8, -6,
-5, -4, -2, -1, 1, 2, 4, 5, 6, 7, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
21, 22, 22, 23, 24, 25, 25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 33, 34, 35, 37,
38, 38, 39, 40, 41, 42, 42, 43, 44, 45, 46, 47, 47, 48, 49, 50, 50, 51, 52, 52,
53, 53, 54, 54, 55, 55, 55, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 55, 55,
55, 54, 53, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 39, 38, 37, 36,
35, 33, 32, 31, 29, 28, 27, 26, 24, 23, 21, 19, 18, 16, 15, 13, 12, 10, 9, 7, 6,
4, 3, 1, 0, -2, -3, -5, -6, -7, -9, -10, -11, -13, -14, -15, -16, -17, -18, -19,
-19, -20, -21, -22, -23, -23, -24, -24, -25, -25, -26, -26, -27, -27, -27, -28,
-28, -28, -28, -29, -29, -29, -29, -29, -29, -29, -29, -30, -30, -30, -30, -30,
-30, -30, -30, -30, -30, -30, -31, -31, -31, -31, -32, -32, -32, -32, -33, -33,
-33, -33, -34, -34, -34, -34, -35, -35, -35, -35, -36, -36, -36, -36, -36, -36,
-37, -37, -37, -37, -38, -38, -38, -38, -38, -38, -38, -38, -38, -37, -37, -37,
-37, -36, -36, -36, -35, -35, -34, -34, -33, -33, -32, -32, -31, -30, -29, -29,
-28, -27, -26, -25, -24, -23, -23, -22, -21, -19, -18, -17, -16, -14, -13, -12,
-11, -10, -9, -8, -7, -6, -5, -3, -2, -1, 0, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
13, 14, 15, 16, 16, 17, 18, 19, 19, 20, 21, 21, 22, 22, 23, 23, 24, 24, 25, 25,
25, 26, 26, 26, 27, 27, 27, 27, 27, 27, 28, 28, 28, 28, 28, 28, 28, 28, 29, 29,
29, 29, 29, 30, 30, 30, 31, 31, 31, 32, 32, 32, 32, 33, 33, 32, 33, 34, 34, 34,
34, 35, 35, 35, 35, 35, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36,
36, 36, 35, 35, 35, 34, 34, 34, 33, 33, 32, 32, 31, 30, 30, 29, 28, 27, 27, 26,
25, 24, 23, 23, 22, 21, 20, 19, 18, 17, 16, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7,
5, 4, 3, 2, 1, 0, -1, -2, -3, -4, -6, -7, -8, -9, -10, -11, -12, -13, -14, -15,
-16, -17, -18, -19, -20, -21, -22, -22, -23, -24, -24, -25, -26, -26, -26, -27,
-27, -28, -28, -28, -28, -29, -29, -29, -29, -29, -29, -29, -29, -29, -29, -29,
-29, -29, -29, -29, -29, -29, -28, -28, -28, -28, -28, -28, -27, -27, -27, -27,
-26, -26, -26, -26, -26, -26, -25, -25, -25, -25, -25, -24, -24, -24, -24, -24,
-24, -24, -24, -24, -24, -24, -24, -23, -23, -23, -23, -23, -23, -23, -23, -23,
-23, -23, -23, -23, -23, -23, -23, -23, -23, -23, -23, -23, -22, -22, -22, -22,
-22, -21, -21, -21, -21, -20, -20, -20, -19, -19, -18, -18, -17, -17, -17, -16,
-16, -15, -14, -14, -13, -13, -12, -12, -11, -10, -10, -9, -8, -7, -7, -6, -5,
-4, -3, -2, -1, -1, 0, 1, 2, 2, 3, 4, 4, 5, 6, 7, 8, 8, 9, 10, 11, 11, 12, 13,
14, 14, 15, 15, 16, 16, 17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 22, 22,
23, 23, 23, 23, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 25, 25, 25, 25, 25, 25,
25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25,
25, 25, 25, 24, 24, 24, 24, 24, 23, 23, 23, 22, 22, 22, 21, 21, 20, 20, 19, 19,
18, 17, 17, 16, 16, 15, 14, 14, 13, 12, 12, 11, 10, 10, 9, 9, 8, 7, 7, 6, 5, 5,
4, 3, 2, 2, 1, 0, -1, -2, -3, -3, -4, -5, -6, -6, -7, -8, -8, -9, -10, -11, -11,
-12, -13, -14, -14, -15, -16, -16, -17, -18, -18, -19, -19, -20, -21, -21, -22,
-22, -22, -23, -23, -24, -24, -24, -24, -25, -25, -25, -25, -25, -26, -26, -26,
-26, -26, -26, -26, -26, -26, -26, -26, -26, -26, -26, -25, -25, -25, -25, -25,
-25, -25, -24, -24, -24, -24, -24, -24, -23, -23, -23, -23, -23, -22, -22, -22,
-22, -22, -22, -21, -21, -21, -21, -21, -20, -20, -20, -20, -20, -20, -19, -19,
-19, -19, -19, -19, -18, -18, -18, -18, -18, -18, -18, -17, -17, -17, -17, -17,
-17, -17, -16, -16, -16, -16, -16, -15, -15, -15, -15, -14, -14, -14, -13, -13,
-13, -12, -12, -12, -11, -11, -10, -10, -10, -9, -9, -8, -8, -7, -7, -6, -6, -5,
-5, -4, -4, -3, -3, -2, -2, -1, 0, 0, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8,
8, 9, 10, 10, 11, 11, 12, 12, 12, 13, 13, 14, 14, 15, 15, 15, 16, 16, 16, 17,
17, 17, 18, 18, 18, 18, 19, 19, 19, 19, 19, 20, 20, 20, 20, 20, 20, 20, 20, 20,
20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19,
18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 17, 17, 17, 17, 17, 16, 16, 16, 15, 15,
15, 14, 14, 13, 13, 13, 12, 12, 11, 11, 10, 10, 10, 9, 9, 8, 8, 7, 7, 6, 6, 5,
5, 4, 4, 3, 3, 2, 2, 1, 1, 0, 0, -1, -1, -2, -2, -3, -3, -4, -5, -5, -6, -6, -7,
-7, -8, -8, -9, -9, -10, -10, -11, -11, -12, -12, -12, -13, -13, -14, -14, -15,
-15, -15, -16, -16, -17, -17, -17, -18, -18, -18, -19, -19, -19, -19, -19, -20,
-20, -20, -20, -20, -20, -21, -21, -21, -21, -21, -21, -21, -21, -21, -21, -21,
-21, -21, -21, -21, -20, -20, -20, -20, -20, -20, -20, -20, -20, -20, -19, -19,
-19, -19, -19, -19, -19, -19, -18, -18, -18, -18, -18, -18, -18, -17, -17, -17,
-17, -17, -17, -16, -16, -16, -16, -16, -16, -15, -15, -15, -15, -15, -15, -14,
-14, -14, -14, -14, -13, -13, -13, -13, -13, -12, -12, -12, -12, -11, -11, -11,
-10, -10, -10, -10, -9, -9, -9, -8, -8, -8, -7, -7, -7, -6, -6, -5, -5, -5, -4,
-4, -4, -3, -3, -3, -2, -2, -1, -1, -1, 0, 0, 1, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5,
5, 6, 6, 6, 7, 7, 8, 8, 8, 9, 9, 9, 10, 10, 11, 11, 11, 11, 12, 12, 12, 13, 13,
13, 13, 14, 14, 14, 14, 15, 15, 15, 15, 15, 15, 16, 16, 16, 16, 16, 16, 16, 16,
16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 14, 14, 14, 14, 14, 14,
14, 14, 14, 14, 13, 13, 13, 13, 13, 13, 13, 13, 12, 12, 12, 12, 12, 11, 11, 11,
11, 10, 10, 10, 10, 9, 9, 9, 8, 8, 8, 7, 7, 7, 6, 6, 6, 5, 5, 5, 4, 4, 4, 3, 3,
2, 2, 2, 1, 1, 1, 0, 0, 0, -1, -1, -2, -2, -2, -3, -3, -3, -4, -4, -4, -5, -5,
-6, -6, -6, -6, -7, -7, -7, -8, -8, -8, -9, -9, -9, -10, -10, -10, -11, -11,
-11, -12, -12, -12, -12, -13, -13, -13, -13, -14, -14, -14, -14, -14, -15, -15,
-15, -15, -15, -15, -15, -16, -16, -16, -16, -16, -16, -16, -16, -16, -16, -16,
-16, -16, -16, -16, -16, -16, -16, -16, -16, -16, -16, -16, -16, -16, -16, -16,
-16, -16, -15, -15, -15, -15, -15, -15, -15, -15, -15, -15, -15, -14, -14, -14,
-14, -14, -14, -14, -14, -13, -13, -13, -13, -13, -13, -12, -12, -12, -12, -12,
-12, -11, -11, -11, -11, -11, -10, -10, -10, -10, -9, -9, -9, -9, -8, -8, -8,
-8, -7, -7, -7, -6, -6, -6, -6, -5, -5, -5, -5, -4, -4, -4, -3, -3, -3, -3, -2,
-2, -2, -1, -1, -1, 0, 0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5, 5, 5, 5,
6, 6, 6, 6, 7, 7, 7, 8, 8, 8, 8, 8, 9, 9, 9, 9, 10, 10, 10, 10, 10, 11, 11, 11,
11, 11, 11, 12, 12, 12, 12, 12, 12, 12, 12, 13, 13, 13, 13, 13, 13, 13, 13, 13,
13, 13, 13, 13, 13, 13, 13, 13, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
12, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 10, 10, 10, 10, 10, 10, 10, 10,
10, 9, 9, 9, 9, 9, 9, 9, 9, 8, 8, 8, 8, 8, 8, 7, 7, 7, 7, 7, 6, 6, 6, 6, 5, 5,
5, 5, 5, 4, 4, 4, 4, 3, 3, 3, 3, 2, 2, 2, 1, 1, 1, 1, 0, 0, 0, 0, -1, -1, -1,
-1, -2, -2, -2, -3, -3, -3, -3, -4, -4, -4, -4, -5, -5, -5, -5, -6, -6, -6, -6,
-7, -7, -7, -7, -7, -8, -8, -8, -8, -9, -9, -9, -9, -9, -10, -10, -10, -10, -10,
-10, -11, -11, -11, -11, -11, -11, -11, -11, -12, -12, -12, -12, -12, -12, -12,
-12, -12, -12, -12, -12, -12, -12, -12, -12, -12, -12, -12, -12, -12, -12, -12,
-12, -12, -12, -12, -12, -12, -12, -11, -11, -11, -11, -11, -11, -11, -11, -11,
-11, -11, -11, -10, -10, -10, -10, -10, -10, -10, -10, -10, -9, -9, -9, -9, -9,
-9, -9, -8, -8, -8, -8, -8, -8, -7, -7, -7, -7, -7, -7, -7, -6, -6, -6, -6, -6,
-6, -5, -5, -5, -5, -5, -4, -4, -4, -4, -4, -4, -3, -3, -3, -3, -3, -2, -2, -2,
-2, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3,
4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 6, 6, 6, 6, 6, 6, 7, 7, 7, 7, 7, 7, 7, 8, 8, 8,
8, 8, 8, 8, 8, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 6, 6, 6, 6, 6, 6, 6, 6, 5, 5, 5, 5, 5, 5, 5, 5, 4,
4, 4, 4, 4, 4, 3, 3, 3, 3, 3, 3, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
-1, -1, -1, -1, -1, -2, -2, -2, -2, -2, -3, -3, -3, -3, -3, -4, -4, -4, -4, -4,
-4, -5, -5, -5, -5, -5, -6, -6, -6, -6, -6, -6, -7, -7, -7, -7, -7, -7, -7, -8,
-8, -8, -8, -8, -8, -8, -8, -8, -9, -9, -9, -9, -9, -9, -9, -9, -9, -9, -9, -9,
-9, -9, -9, -9, -9, -9, -9, -9, -9, -9, -9, -9, -9, -9, -9, -9, -9, -9, -9, -9,
-9, -9, -9, -9, -8, -8, -8, -8, -8, -8, -8, -8, -8, -8, -8, -8, -7, -7, -7, -7,
-7, -7, -7, -7, -6, -6, -6, -6, -6, -6, -6, -6, -6, -5, -5, -5, -5, -5, -5, -5,
-4, -4, -4, -4, -4, -4, -4, -4, -3, -3, -3, -3, -3, -3, -3, -2, -2, -2, -2, -2,
-2, -2, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1,
1, 1, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5,
5, 5, 5, 5, 5, 5, 5, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 7, 7, 7, 7,
7, 7, 7, 7, 7, 7, 7, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 5, 5, 5,
5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 3, 3,
3, 3, 3, 3, 3, 3, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0,
0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -2, -2, -2, -2, -2, -2, -2, -2,
-3, -3, -3, -3, -3, -3, -3, -3, -3, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -5,
-5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -6, -6, -6, -6, -6, -6,
-6, -6, -6, -6, -6, -6, -6, -6, -6, -6, -6, -6, -6, -6, -6, -6, -6, -6, -6, -6,
-6, -6, -6, -6, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5,
-5, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -3, -3, -3, -3, -3,
-3, -3, -3, -3, -3, -3, -3, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -1,
-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 2, 2,
2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1,
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
-1, -1, -1, -1, -1, -1, -1, -1, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -1, -1,
-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
-1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  };
