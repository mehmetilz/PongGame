/*
    1.Create a blank screen and game loop (boş bir ekran yarat ve oyun döngüsünü ayarla)+
    2.Draw the paddles and the ball (kürekleri ve toplu çiz)+
    3.Move the ball around (topu hareket ettir)+
    4.Check for a collision with all edges (tüm kenarlarla çarpışıyor mu ? kontrol et)
    5.Move the players paddle (oyuncunun küreğini hareket ettir)
    6.Move the CPU paddle with AI (CPU küreğini AI ile kontrol et)
    7.Check the collision with the paddle (küreklerle çarpışıyor mu kontrol et)
    8.Add the scoring (puanlama ekle)
*/
/*
void void DrawRectangle(int posX, int posY, int width, int height, Color color); (DİKDÖRTGEN)
void DrawCircle(int centerX, int centerY, float radius, Color color); (DAİRE)
void DrawLine(int startPosX, int startPosY, int end PosX, int end PosY, Color color); (ÇİZGİ) 
void DrawPoly(Vector2 center, int sides, float radius, float rotation, Color color); (kenarını kendimizin belirlediği ÇOKGEN)
  */
 #include <raylib.h>
 #include <stdio.h>
 #include <stdlib.h>
 
 Color green = {38, 185, 154, 255};
 Color dark_green = {20, 163, 133, 255};
 Color light_green = {129, 204, 184, 255};
 Color yellow = {243, 213, 91, 255};

 int player_score = 0;
 int cpu_score = 0;

 typedef struct ball{
    float x, y;
    int speed_x, speed_y;
    int radius;
    
     void (*draw)(struct ball* self);
     void (*update)(struct ball* self, int* player_score, int* cpu_score);
 }ball;
     void drawball(ball* self){
         DrawCircle(self->x, self->y, self->radius , yellow);
        };
 
        void updateBall(ball* self, int* player_score, int* cpu_score) {
            self->x += self->speed_x;
            self->y += self->speed_y;
        
            // Üst ve alt duvara çarpma
            if (self->y + self->radius >= 800 || self->y - self->radius <= 0) {
                self->speed_y *= -1;
            }
        
            // Eğer sol tarafa giderse CPU puan alır
            if (self->x - self->radius <= 0) {
                (*cpu_score)++;
                self->x = 600; // Ortaya geri koy
                self->y = 400;
                self->speed_x = (rand() % 2 == 0) ? 4 : -4;
                self->speed_y = (rand() % 2 == 0) ? 4 : -4;
            }
        
            // Eğer sağ tarafa giderse Oyuncu puan alır
            if (self->x + self->radius >= 1200) {
                (*player_score)++;
                self->x = 600;
                self->y = 400;
                self->speed_x = (rand() % 2 == 0) ? 4 : -4;
                self->speed_y = (rand() % 2 == 0) ? 4 : -4;
            }
        }
        
 
     typedef struct paddle {
         float width, height;
         int speed;
         float x, y;
 
         void (*draw)(struct paddle* self);
         void (*update)(struct paddle* self, float ball_y);
         void (*limit)(struct paddle* self);
     }paddle;

     void limit(paddle* self){
        if(self->y <= 0 ){       
            self->y = 0;
        }
        if(self->y + self->height >= 800 ){
            self->y = 800 - self->height;
        }
    }
 
     void drawPaddle(paddle* self){
        Rectangle rect = {self->x, self->y, self->width, self->height};  // Yapıyı düzgün başlatıyoruz
        DrawRectangleRounded(rect, 0.8, 0, WHITE); 
     }
 
     void updatePaddle(paddle* self, float ball_y){
         if(IsKeyDown(KEY_UP)){
             self->y -= self->speed;
         }
         if(IsKeyDown(KEY_DOWN)){
             self->y += self->speed;
         }
         limit(self);
     };
 
     void updateCpuPaddle (paddle* self , float ball_y){
         if(self->y + self->height/2 < ball_y){
            self->y += self->speed;
         }else if(self->y + self->height/2 >= ball_y){
            self->y -= self->speed;
         }
         limit(self);
    };

    void checkCollision(ball* myball, paddle* player) {
        if (CheckCollisionCircleRec(
            (Vector2){myball->x, myball->y}, myball->radius,
            (Rectangle){player->x, player->y, player->width, player->height})) 
        {
            myball->speed_x *= -1; 
        }
    }
    
int main(){
    
     const int screen_width=1200;
     const int screen_height=800;
     InitWindow(screen_width,screen_height,"My Pong Game!");
     SetTargetFPS(60);
 
 
     ball myball = {screen_width/2, screen_height/2, 4, 4, 10, drawball, updateBall };
 
     paddle player = {25, 120, 7, screen_width - player.width -10 ,
          (screen_height/2) - (player.height/2), drawPaddle, updatePaddle};
 
     paddle cpu ={25, 120, 7, 10, 
         (screen_height/2) - (player.height/2), drawPaddle, updateCpuPaddle};   
        
    
 // Check for events.
 // Update the position of game objects.
 // Draw the game objects in their new position.

    while (!WindowShouldClose()){  

    float ball_y = myball.y;

     BeginDrawing();
 
 // Updating
     updateBall(&myball, &player_score, &cpu_score);
     myball.update(&myball, &player_score, &cpu_score);
     player.update(&player, ball_y);
     cpu.update(&cpu, ball_y);

 // Check for collision    
    checkCollision(&myball, &player);
    checkCollision(&myball, &cpu);
 
 // Drawing
     ClearBackground(dark_green);
     DrawCircle(600, 400, 150 , light_green);
     DrawLine(screen_width/2, 0, screen_width/2, screen_height, WHITE);
     myball.draw(&myball);
     player.draw(&player);
     cpu.draw(&cpu);
     DrawText(TextFormat("%d", player_score), 300, 40, 80, WHITE);
     DrawText(TextFormat("%d", cpu_score), 900, 40, 80, WHITE);
  
     EndDrawing();
    }
 
     CloseWindow();
     return 0;
 }