#ifndef GAME_FRAME_H_
#define GAME_FRAME_H_

#include "Frame/healthbar.h"
#include "Frame/aim.h"

#include "window.h"

#include "gltext.h"
#include <stdio.h>

class GameFrame : public HealthBar, public Aim
{
        GLTtext *text;
public:
        int score;
        bool is_end;

        GameFrame (void)
        : score(0), is_end(false)
        {
                text = gltCreateText();
                reset_text();
        }

        void
        reset_text (void) {
                char buf[80];
                if (!is_end) sprintf(buf, "Score: %d", score);
                else sprintf(buf, "Game Over!\nYour score is %d.\nPress ESC to quit.", score);
                gltSetText(text, buf);
        }

        void
        draw (void) {
                if (!is_end) {
                        HealthBar :: draw();
                        Aim :: draw();
                }

                gltBeginDraw();
                gltColor(1.0f, 1.0f, 1.0f, 0.0f);
                //gltDrawText2D(text, 10, 30, 3);
                gltDrawText2DAligned(text, Window::get_width() / 2, 50, 3, GLT_CENTER, GLT_TOP);
                gltEndDraw();

        
        }

        void end_game (void) {
                is_end = true;
        }

        ~GameFrame (void) {
                gltDeleteText(text);
        }
};

#endif