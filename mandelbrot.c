#import <stdio.h>
#import <math.h>

#define MAX_I           1000
#define MAX_X           640
#define MAX_Y           480

#define PALETTE_SIZE    500
#define START_COLOR     {   3,  18,  33 }
#define MIDDLE_COLOR    { 255, 255, 255 }
#define END_COLOR       { 254, 180,  28 }

int lasty = 1;

typedef struct {
    float red;
    float green;
    float blue;
} Color;

void paletteFromTo(Color colstart, Color colend, Color palette[], int start, int max);
void generatePalette(Color palette[]);
void initPPM();
void plot(int x, int y, Color c); 

void generatePalette(Color palette[]) {
    Color start = START_COLOR;
    Color middle = MIDDLE_COLOR;
    Color end = END_COLOR;
    
    paletteFromTo(start, middle, palette, 0, PALETTE_SIZE / 2);
    paletteFromTo(middle, end, palette, PALETTE_SIZE / 2, PALETTE_SIZE);
}

void paletteFromTo(Color colstart, Color colend, Color palette[], int start, int max) {
    int cnt = max - start;
    float rfact = (float)(colend.red - colstart.red) / cnt;
    float gfact = (float)(colend.green - colstart.green) / cnt;
    float bfact = (float)(colend.blue - colstart.blue) / cnt;
    
    int i;
    for(i = 1; i<=cnt; i++) {
        float red = (float)colstart.red + rfact * (float)i;
        float green = (float)colstart.green + gfact * (float)i;
        float blue = (float)colstart.blue + bfact * (float)i;
        Color current = { red, green, blue };
        palette[start+i-1] = current;
        //printf("%f %f %f\n", current.red, current.green, current.blue);
    }
}

void initPPM() {
    // # The P3 means colors are in ASCII, then x columns and y rows,
    // then 255 for max color
    printf("P3\n");
    printf("%d %d\n", (int)MAX_X, (int)MAX_Y);
    printf("255\n");
}

void plot(int x, int y, Color c) {
    if(lasty != y) {
        printf("\n");
        lasty = y;
    }
    printf("%03d %03d %03d ", (int)(c.red), (int)(c.green), (int)(c.blue));
}

int main() {
    int sy, sx;
    
    Color palette[PALETTE_SIZE];
    generatePalette(palette);
    initPPM();
        
    for(sy = 1; sy <= MAX_Y; sy++) {
        for(sx = 1; sx <= MAX_X; sx++) {
            float x, y;
            x = 0.0;
            y = 0.0;
            
            // x min -2.5, max 1.0
            // y min -1.0, max 1.0

            float x0 = (sx / (float)MAX_X * 3) - 2;
            float y0 = (sy / (float)MAX_Y * 2.0) - 1.0;
           
            int i = 0;
            while(((x*x - y*y) < (2*2)) && i < MAX_I) {
                float xtmp;
                xtmp = x*x - y*y + x0;
                y = 2*x*y + y0;
                x = xtmp;
                i++;
            }
            
            Color c = { 0, 0, 0 };
            if((x*x + y*y) > (2*2)) {
                c = palette[i % PALETTE_SIZE];
            }
            plot(sx, sy, c);
        }
    }
    
    return 0;
}
