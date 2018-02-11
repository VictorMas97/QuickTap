/*
 * Autor: Víctor
 * Fecha: 11/02/2018
 * Copyright (c) 2018 Víctor
 */
#include "Piece.hpp"
#include <basics/Canvas>
#include <basics/Director>
#include <basics/Log>

using namespace basics;
using namespace QuickTap;

namespace QuickTap
{

    const float Piece::match_range = 10;

    Piece::Cut_Result Piece::cut (const Piece & other)
    {
        Cut_Result result;

        result.second = *this;          // De entrada copiamos todos los atributos de this en el resultado.
                                        // Luego cambiaremos solo los que sea necesario.
        if (speed[0] != 0.f)
        {
            float  this_min_x  =  this->position[0];
            float  this_max_x  =  this_min_x + this->size[0];
            float  other_min_x =  other.position[0];
            float  other_max_x =  other_min_x + other.size[0];

            if (this_max_x < other_min_x || this_min_x > other_max_x)       // No se solapan
            {
                result.first = OUTSIDE;
            }

            else                                                            // Se solapan, pero hay que comprobar si perfectamente o no
            {
                float delta_min_x = other_min_x - this_min_x;
                float delta_max_x = other_max_x - this_max_x;

                if (fabsf (delta_min_x) < match_range)                      // Se solapan perfectamente (Por el lado minimo)
                {
                    result.first = EXACT_MATCH;
                    this->position[0] = other.position[0];
                }

                else if (fabsf (delta_max_x) < match_range)                 // Se solapan perfectamente (Por el lado maximo)
                {
                    result.first = EXACT_MATCH;
                    this->position[0] = other.position[0] + other.size[0] - this->size[0];
                }

                else                                                        // No se solapan perfectamente: hay que cortar
                {
                    result.first = CUTTED;

                    Piece & remainder = result.second;

                    if (delta_min_x > 0.f)
                    {
                        position[0] = other_min_x;
                        size[0]     = this_max_x - other_min_x;

                        remainder.size[0] = delta_min_x;
                    }

                    else
                    {
                        size[0] = other_max_x - this_min_x;

                        remainder.position[0] = other_max_x;
                        remainder.size[0] = -delta_min_x;
                    }
                }
            }
        }

        else if (speed[2] != 0.f)
        {
            float  this_min_z  =  this->position[2];
            float  this_max_z  =  this_min_z + this->size[2];
            float  other_min_z =  other.position[2];
            float  other_max_z =  other_min_z + other.size[2];

            if (this_max_z < other_min_z || this_min_z > other_max_z)          // No se solapan
            {
                result.first = OUTSIDE;
            }

            else                                                               // Se solapan, pero hay que comprobar si perfectamente o no
            {
                float delta_min_z = other_min_z - this_min_z;
                float delta_max_z = other_max_z - this_max_z;

                if (fabsf (delta_min_z) < match_range)                         // Se solapan perfectamente (Por el lado maximo)
                {
                    result.first = EXACT_MATCH;
                    this->position[2] = other.position[2];
                }

                else if (fabsf (delta_max_z) < match_range)                    // Se solapan perfectamente (Por el lado maximo)
                {
                    result.first = EXACT_MATCH;
                    this->position[2] = other.position[2] + other.size[2] - this->size[0];
                }

                else                                                           // No se solapan perfectamente: hay que cortar
                {
                    result.first = CUTTED;

                    Piece & remainder = result.second;

                    if (delta_min_z > 0.f)
                    {
                        position[2] = other_min_z;
                        size[2]     = this_max_z - other_min_z;

                        remainder.size[2] = delta_min_z;
                    }

                    else
                    {
                        size[2] = other_max_z - this_min_z;

                        remainder.position[2] = other_max_z;
                        remainder.size[2] = -delta_min_z;
                    }
                }
            }
        }

        return result;
    }
}
