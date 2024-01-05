#pragma once

#include <random>
#include <chrono>
#include <algorithm>
#include <functional>
#include "mino_factory.h"
#include "log.h"

enum Type {LINE, Z, RZ, L, RL, T, SQUARE};
enum Orientation {O1, O2, O3, O4};

//const uint32_t seed = std::chrono::system_clock::now().time_since_epoch().count();
static std::random_device rd;
static std::default_random_engine engine(rd());

class DefaultFactory: public Factory {
private:
    
    void create_mino_internal (
        Type type, 
        Orientation orientation, 
        Color color, 
        int pos_x, 
        int pos_y, 
        int sz, 
        Mino& mino) {
    mino.reserve(4);
    
    switch (type) {
        case LINE: 
            switch (orientation) {
                case O1:
                    mino.assign({
                            {type, O1, color, {pos_x, pos_y}}, 
                            {type, O1, color, {pos_x+sz, pos_y}}, 
                            {type, O1, color, {pos_x+2*sz, pos_y}},
                            {type, O1, color, {pos_x+3*sz, pos_y}}
                    });
                    break;
                case O2:
                    mino.assign({
                        {type, O2, color, {pos_x, pos_y}},
                        {type, O2, color, {pos_x, pos_y+sz}},
                        {type, O2, color, {pos_x, pos_y+2*sz}},
                        {type, O2, color, {pos_x, pos_y+3*sz}}
                    });
                default: break;
            }
            break;
        case Z:
            switch (orientation) {
                case O1:
                    mino.assign({
                        {type, O1, color, {pos_x, pos_y}},
                        {type, O1, color, {pos_x+sz, pos_y}},
                        {type, O1, color, {pos_x+sz, pos_y+sz}},
                        {type, O1, color, {pos_x+2*sz, pos_y+sz}}
                    });
                    break;
                case O2:
                    mino.assign({
                        {type, O2, color, {pos_x, pos_y}},
                        {type, O2, color, {pos_x, pos_y+sz}},
                        {type, O2, color, {pos_x-sz, pos_y+sz}},
                        {type, O2, color, {pos_x-sz, pos_y+2*sz}}
                    });
                default: break;
            }
            break;    
        case RZ:
            switch (orientation) {
                case O1:
                    mino.assign({
                        {type, O1, color, {pos_x, pos_y}},
                        {type, O1, color, {pos_x-sz, pos_y}},
                        {type, O1, color, {pos_x-sz, pos_y+sz}},
                        {type, O1, color, {pos_x-2*sz, pos_y+sz}}
                    });
                    break;
                case O2:
                    mino.assign({
                        {type, O2, color, {pos_x, pos_y}},
                        {type, O2, color, {pos_x, pos_y+sz}},
                        {type, O2, color, {pos_x+sz, pos_y+sz}},
                        {type, O2, color, {pos_x+sz, pos_y+2*sz}}
                    });
                default: break;
            }
            break;
        case L:
            switch (orientation) {
                case O1:
                    mino.assign({
                        {type, O1, color, {pos_x, pos_y}},
                        {type, O1, color, {pos_x, pos_y+sz}},             //            |
                        {type, O1, color, {pos_x-sz, pos_y+sz}},          //      ______|
                        {type, O1, color, {pos_x-2*sz, pos_y+sz}}
                    });
                    break;
                case O2:
                    mino.assign({
                        {type, O2, color, {pos_x, pos_y}},             
                        {type, O2, color, {pos_x, pos_y+sz}},          //      |    
                        {type, O2, color, {pos_x, pos_y+2*sz}},        //      |__
                        {type, O2, color, {pos_x+sz, pos_y+2*sz}}
                    });
                    break;
                case O3:
                    mino.assign({
                        {type, O3, color, {pos_x, pos_y}},             //       ______  
                        {type, O3, color, {pos_x, pos_y-sz}},          //      |    
                        {type, O3, color, {pos_x+sz, pos_y-sz}},       
                        {type, O3, color, {pos_x+2*sz, pos_y-sz}}
                    });
                    break;
                case O4:
                    mino.assign({
                        {type, O4, color, {pos_x, pos_y}},             //      __  
                        {type, O4, color, {pos_x+sz, pos_y}},          //        |    
                        {type, O4, color, {pos_x+sz, pos_y+sz}},       //        | 
                        {type, O4, color, {pos_x+sz, pos_y+2*sz}}
                    });
            }
            break;
        case RL:
            switch (orientation) {
                case O1:
                    mino.assign({
                        {type, O1, color, {pos_x, pos_y}},
                        {type, O1, color, {pos_x, pos_y+sz}},         //         |
                        {type, O1, color, {pos_x+sz, pos_y+sz}},      //         |_______ 
                        {type, O1, color, {pos_x+2*sz, pos_y+sz}}
                    });
                    break;
                case O2:
                    mino.assign({
                        {type, O2, color, {pos_x, pos_y}},
                        {type, O2, color, {pos_x-sz, pos_y}},         //         __
                        {type, O2, color, {pos_x-sz, pos_y+sz}},      //        | 
                        {type, O2, color, {pos_x-sz, pos_y+2*sz}}     //        |  
                    });
                    break;
                case O3:
                    mino.assign({
                        {type, O3, color, {pos_x, pos_y}},
                        {type, O3, color, {pos_x+sz, pos_y}},         //          ____
                        {type, O3, color, {pos_x+2*sz, pos_y}},       //              |    
                        {type, O3, color, {pos_x+2*sz, pos_y+sz}}            
                    });
                    break;
                case O4:
                    mino.assign({
                        {type, O4, color, {pos_x, pos_y}},
                        {type, O4, color, {pos_x, pos_y+sz}},         //          |
                        {type, O4, color, {pos_x, pos_y+2*sz}},       //        __|    
                        {type, O4, color, {pos_x-sz, pos_y+2*sz}}     //          
                    });
            }
            break;
        case T:
            switch (orientation) {
                case O1:
                    mino.assign({
                        {type, O1, color, {pos_x, pos_y}},
                        {type, O1, color, {pos_x-sz, pos_y+sz}},      
                        {type, O1, color, {pos_x, pos_y+sz}},    //      __|__
                        {type, O1, color, {pos_x+sz, pos_y+sz}},
                    });
                    break;
                case O2:
                    mino.assign({
                        {type, O2, color, {pos_x, pos_y}},
                        {type, O2, color, {pos_x, pos_y+sz}},      
                        {type, O2, color, {pos_x, pos_y+2*sz}},    //      |_
                        {type, O2, color, {pos_x+sz, pos_y+sz}}   //       |   
                    });
                    break;
                case O3:
                    mino.assign({
                        {type, O3, color, {pos_x, pos_y}},
                        {type, O3, color, {pos_x+sz, pos_y}},      //     _____
                        {type, O3, color, {pos_x+2*sz, pos_y}},    //       |
                        {type, O3, color, {pos_x+sz, pos_y+sz}}
                    });
                    break;
                case O4:
                    mino.assign({
                        {type, O4, color, {pos_x, pos_y}},
                        {type, O4, color, {pos_x, pos_y+sz}},      //    _|
                        {type, O4, color, {pos_x, pos_y+2*sz}},    //     |  
                        {type, O4, color, {pos_x-sz, pos_y+sz}}
                    });
            }
            break;
        case SQUARE: default:
            mino.assign({
                {type, O1, color, {pos_x, pos_y}},
                {type, O1, color, {pos_x+sz, pos_y}},
                {type, O1, color, {pos_x, pos_y+sz}},
                {type, O1, color, {pos_x+sz, pos_y+sz}}
            });
    }
        }
public:
    DefaultFactory() {}

    void create_mino(int sq_per_line, int sz, Mino& mino) {
        

        std::uniform_int_distribution<> gen_types(LINE, SQUARE);
        std::uniform_int_distribution<> gen_pos(4, sq_per_line-4);

        Type type = (Type)gen_types(engine);

        Point tl {gen_pos(engine)*sz, 0};

        Color color = {255,0,0};
        std::shuffle(color.begin(), color.end(), engine);
        
        create_mino_internal(type, O1, color, tl[0], tl[1], sz, mino);
    }

    void rotate(const Mino& input, int sz, Mino& out) {

        const Brick& brick = input[0];

        switch(brick.type) {
            case LINE: case Z: case RZ:
                switch(brick.orientation) {
                    case O1: create_mino_internal((Type)brick.type, O2, brick.color, brick.tl[0], brick.tl[1], sz, out); break;
                    case O2: create_mino_internal((Type)brick.type, O1, brick.color, brick.tl[0], brick.tl[1], sz, out); break;
                }
                break;
            case L: case RL: case T:
                switch(brick.orientation) {
                    case O1: create_mino_internal((Type)brick.type, O2, brick.color, brick.tl[0], brick.tl[1], sz, out); break;
                    case O2: create_mino_internal((Type)brick.type, O3, brick.color, brick.tl[0], brick.tl[1], sz, out); break;
                    case O3: create_mino_internal((Type)brick.type, O4, brick.color, brick.tl[0], brick.tl[1], sz, out); break;
                    case O4: create_mino_internal((Type)brick.type, O1, brick.color, brick.tl[0], brick.tl[1], sz, out); break;
                }
                break;
            case SQUARE: default: out=input;
        }
    }
};