
#pragma once

const int TILE_SIZE = 80;

// Gameplay map is 1400 x 810.
// UI starts at y = 810.
// With 80px tiles:
// 1400 / 80 = 17.5, so we use 18 columns.
// 810 / 80 = 10.125, so we use 10 rows for tower placement area.
const int ROWS = 10;
const int COLS = 18;

const int MAX_PATH_POINTS = 12;

const int MAX_ENEMIES = 140;
const int MAX_TOWERS = 80;
const int MAX_BULLETS = 320;
const int MAX_ROCKS = 16;