/*
 * Autor: Víctor
 * Fecha: 11/02/2018
 * Copyright (c) 2018 Víctor
 */

#include <memory>
#include <utility>
#include <basics/Canvas>
#include <basics/Texture_2D>
#include <basics/Size>
#include <basics/Point>
#include <basics/Vector>
#include "Color.hpp"

namespace QuickTap
{

    using basics::Canvas;
    using basics::Size3f;
    using basics::Point3f;
    using basics::Vector3f;

    class Piece
    {
    public:

        ///Representa el estado de la pieza.

        enum Match
        {
            EXACT_MATCH,
            CUTTED,
            OUTSIDE
        };

        typedef std::pair< Match, Piece > Cut_Result;  /// Un par de cosas, un estado y una pieza.

        static const float match_range;                /// Un rango para que no tengas que poner la pieza exactamente encima de la otra

        Point3f  position;                             /// Un punto en 3 dimensines
        Size3f   size;                                 /// Un tamaño en 3 dimensiones
        Vector3f speed;                                /// Un vector en 3 dimensiones
        Color    color;                                /// Un color

    public:

        Piece()                                        /// Construye una pieza vacia.
        {
           size = { 0, 0, 0 };
        }

        Piece(const Point3f & point, const Size3f & size, const Vector3f & speed, const Color & color)  /// Construye una pieza con sus atributos.
        {
            this->position = point;
            this->size     = size;
            this->speed    = speed;
            this->color    = color;
        }

        void set_color (const Color & new_color)       /// Pone color a la pieza.
        {
            this->color = new_color;
        }

        void stop ()                                   /// Para la pieza.
        {
            speed = { 0, 0, 0 };
        }

        void update()                                  /// Mueve la pieza y cuando llega a un bode de la pantalla la velocidad en ese eje la multiplica por menos 1.
        {
            position[0] += speed[0];
            position[2] += speed[2];

            if (speed[0] != 0.f && position[0] >= 1280 - size[0] || position[0] <= 0)
            {
                speed[0] *= -1;
            }

            else if (speed[2] != 0.f && position[2] >= 720 - size[2] || position[2] <= 0)
            {
                speed[2] *= -1;
            }
        }

        void render(Canvas &canvas)                    /// Llama a "set_color" y a "render_2d".
        {
            canvas.set_color (float(color.r) / 255.f, float(color.g) / 255.f, float(color.b) / 255.f);
            render_2d (canvas);
        }

        Cut_Result cut (const Piece & piece);          /// Comprueba si hay que recortar la pieza y si es así la recorta.

    private:

        void render_2d (Canvas & canvas)               /// Pinta la pieza en 2 dimensiones (un cuadrado).
        {
            canvas.fill_rectangle ({ position[0], position[2] }, { size.width, size.depth });
        }

        //void render_isometric (Canvas & canvas);     /// Pinta la pieza en vista isométrica (dos triángulos).

    };

}
