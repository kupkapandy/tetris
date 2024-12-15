#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include "main.h"

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
long double score = 0;

bool init(){
  bool success = true;
  if(SDL_Init(SDL_INIT_VIDEO) < 0){
    printf("couldn't initialize - SDL Error: %s\n", SDL_GetError());
  }else{
    window = SDL_CreateWindow("tetris", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
  }if(window == NULL){
    printf("couldn't create window - SDL Error: %s\n", SDL_GetError());
  }else{
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(renderer == NULL){
      printf("couldn't create renderer - SDL Error: %s\n", SDL_GetError());
    }else{
      SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
      SDL_RenderPresent(renderer);
    }
  }
  return success;
}

void quit(){
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

void drawGrid(uint32_t grid[rows][columns], struct tetromino curr[]){
  SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
  SDL_RenderClear(renderer);
  for(uint32_t i = 2; i < rows; i++){
    bool checkForLine = true;
    for(uint32_t j = 0; j < columns; j++){
      bool isCurrent = false;
      for(uint32_t k = 0; k < 4; k++){
        if(curr[k].y == i && curr[k].x == j){
          isCurrent = true;
          break;
        }
      }
      if(isCurrent){
        SDL_Rect rectangle = (SDL_Rect){j * horizontalSpacing, (i - 2) * verticalSpacing, horizontalSpacing, verticalSpacing};
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0xFF);
        SDL_RenderFillRect(renderer, &rectangle);
        SDL_RenderDrawRect(renderer, &rectangle);
        checkForLine = false;
      }else if(grid[i][j] == 0){
        SDL_Rect rectangle = (SDL_Rect){j * horizontalSpacing, (i - 2) * verticalSpacing, horizontalSpacing, verticalSpacing};
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0x00);
        SDL_RenderDrawRect(renderer, &rectangle);
        checkForLine = false;
      }else if(grid[i][j] == 1){
        SDL_Rect rectangle = (SDL_Rect){j * horizontalSpacing, (i - 2) * verticalSpacing, horizontalSpacing, verticalSpacing};
        SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
        SDL_RenderFillRect(renderer, &rectangle);
        SDL_RenderDrawRect(renderer, &rectangle);
      }
    }
    if(checkForLine){
      clearLine(grid, i);
      score = score * 1.05 + 100;
      printf("Score = %Lf\n", score);
    }
  }
  SDL_RenderPresent(renderer);
}

void clearLine(uint32_t grid[rows][columns], uint32_t indexY){
  for(uint32_t j = 0; j < columns; j++){
    grid[indexY][j] = 0;
  }
  for(uint32_t i = indexY; i > 0; --i){
    bool nothingChanged = true;
    for(uint32_t j = 0; j < columns; ++j){
        grid[i][j] = grid[i - 1][j];
      if(grid[i - 1][j] == 1)
        nothingChanged = false;
    }
    if(nothingChanged && i - 1 > 0){
      for(uint32_t j = 0; j < columns; ++j){
        grid[i][j] = 0;
      }
      break;
    }
  }
}

bool move(uint32_t grid[rows][columns], struct tetromino curr[], uint32_t direction){
  struct tetromino temp[4];
  memcpy(temp, curr, 4 * sizeof(struct tetromino));
  if(direction == 0){
    for(int i = 0; i < 4; i++){
      if(curr[i].y <= 20 && grid[curr[i].y + 1][curr[i].x] == 0) curr[i].y += 1;
      else{
        if(curr[i].y <= 2){ quit(); exit(0);}
        memcpy(curr, temp, 4 * sizeof(struct tetromino));
        return 1;
      }
    }
  }else if(direction == 1){
    for(int i = 0; i < 4; i++){
      if(curr[i].x > 0 && grid[curr[i].y][curr[i].x - 1] == 0) curr[i].x -= 1;
      else{
        memcpy(curr, temp, 4 * sizeof(struct tetromino));
        return 0;
      }
    }
  }else if(direction == 2){
    for(int i = 0; i < 4; i++){
      if(curr[i].x < 9 && grid[curr[i].y][curr[i].x + 1] == 0) curr[i].x += 1;
      else{
        memcpy(curr, temp, 4 * sizeof(struct tetromino));
        return 0;
      }
    }
  }

  return 0;
}

void rotation(uint32_t grid[rows][columns], struct tetromino curr[]){
  struct tetromino temp[4];
  memcpy(temp, curr, 4 * sizeof(struct tetromino));
  for(uint32_t i = 0; i < 4; i++){
    int x = curr[i].y - curr[1].y;
    int y = curr[i].x - curr[1].x;
    curr[i].x = curr[1].x - x;
    curr[i].y = curr[1].y + y;
    if(!(curr[i].x >= 0 && curr[i].x <= 9 && curr[i].y <= 20) || grid[curr[i].y][curr[i].x] != 0){
      memcpy(curr, temp, 4 * sizeof(struct tetromino));
      break;
    }
  }
}


void newTetromino(struct tetromino curr[]){
  switch(rand() % 7 + 1){
    case 1:{ memcpy(curr, I, 4 * sizeof(curr[0])); break;}
    case 2:{ memcpy(curr, O, 4 * sizeof(curr[0])); break;}
    case 3:{ memcpy(curr, T, 4 * sizeof(curr[0])); break;}
    case 4:{ memcpy(curr, J, 4 * sizeof(curr[0])); break;}
    case 5:{ memcpy(curr, L, 4 * sizeof(curr[0])); break;}
    case 6:{ memcpy(curr, S, 4 * sizeof(curr[0])); break;}
    case 7:{ memcpy(curr, Z, 4 * sizeof(curr[0])); break;}
    default:{printf("error! default\n");}
  }
}

void copyToGrid(uint32_t grid[rows][columns], struct tetromino curr[]){
  for(uint32_t i = 0; i < 4; i++){
    grid[curr[i].y][curr[i].x] = 1;
  }
}


int main(){
  srand(time(NULL));
  if(!init()){
    printf("couldn't initialize - SDL Error: %s\n", SDL_GetError());
  }else{
    uint32_t grid[rows][columns];
    memset(grid, 0, sizeof(grid));
    struct tetromino curr[4];
    newTetromino(curr);
    bool quit = false, rotateCheck = true;
    SDL_Event event;
    uint32_t counter = 0;
    while(!quit){
      while(SDL_PollEvent(&event)){
        if(event.type == SDL_QUIT){
          quit = true;
        }
      }
      const Uint8* keyState = SDL_GetKeyboardState(NULL);
      if(keyState[SDL_SCANCODE_DOWN]){
          move(grid, curr, 0);
      }
      if(keyState[SDL_SCANCODE_LEFT]){
          move(grid, curr, 1);
      }
      if(keyState[SDL_SCANCODE_RIGHT]){
          move(grid, curr, 2);
      }
      if(keyState[SDL_SCANCODE_UP] && rotateCheck){
        rotation(grid, curr);
        rotateCheck = false;
      }else rotateCheck = true;
      drawGrid(grid, curr);
      counter += 100;
      if(counter >= 500){
          if(move(grid, curr, 0)){
            copyToGrid(grid, curr);
            newTetromino(curr);
          }
          counter = 0;
      }
      SDL_Delay(85);
    }
  }
  quit();
  return 0;
}
