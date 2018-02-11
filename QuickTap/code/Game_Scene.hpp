/*
 * Autor: Víctor
 * Fecha: 11/02/2018
 * Copyright (c) 2018 Víctor
 */

#include <memory>
#include <vector>
#include <cstdlib>
#include <basics/Scene>
#include <basics/Texture_2D>
#include "Piece.hpp"
#include "Color.hpp"

namespace QuickTap
{
    class Game_Scene : public basics::Scene
    {

        typedef std::shared_ptr< basics::Texture_2D > Texture_Handle;
        typedef std::vector< Piece > Piece_List;

    public:

        ///Representa el estado de la escena en su conjunto.

        enum State
        {
            LOADING,
            RUNNING,
            ERROR
        };

        enum Gameplay_State
        {
            WAITING,
            PLAYING,
			PAUSED,
            GAME_OVER
        };

        enum Direction
        {
            DIRECTION_X,
            DIRECTION_Z
        };

        static const Color    piece_colors[];                                        /// Un array de colores
        static const unsigned piece_color_count;                                     /// Una cuenta de colores

        bool           suspended;                                                    /// True cuando la escena está en segundo plano y viceversa.

        unsigned       canvas_width;                                                 /// Ancho de la resolución virtual usada para dibujar.
        unsigned       canvas_height;                                                /// Alto  de la resolución virtual usada para dibujar.

        Piece          moving_piece;                                                 /// La pieza que se mueve
        Piece_List     stacked_pieces;                                               /// Las piezas que componen la torre
        Piece_List     falling_pieces;                                               /// Las piezas que no componen la torre, menos la que se mueve, es decir, las que se caen

        State          state;                                                        /// El estado de la pieza
		Gameplay_State gameplay_state;                                               /// El estado del juego

        Direction      current_direction;                                            /// La dirección de la pieza

        unsigned       current_color_index;                                          /// El indice que tiene el color actual

        Texture_Handle pause;                                                        /// Textura de la pausa
        Texture_Handle paused_continue;                                              /// Textura de continuar  con el juego
		Texture_Handle paused_leave;                                                 /// Textura de quitar el juego
        Texture_Handle game_over_png;                                                /// Textura del Game Over

    public:

        Game_Scene();                                                                /// Construye una Game_Scene vacia.

        basics::Size2u get_view_size () override                                     /// Debuelve el tamaño de la pantalla.
        {
            return { canvas_width, canvas_height };
        }

        bool initialize () override;                                                 /// Inicializa las variables, creamos la pieza y la pintamos y rellenamos el vector de piezas apiladas.

        void suspend    () override;                                                 /// El juego pasa a segundo plano.

        void resume     () override;                                                 /// El juego pasa a primer plano.

        void handle     (basics::Event & event) override;                            /// Se llama a este método cuando el usuario toca la pantalla, es decir, un input.

        void update     (float time) override;                                       /// Llama a una función u otra dependiendo del estado del juego.

        void render     (basics::Graphics_Context::Accessor & context) override;     /// Se encarga crear un canvas y de pintar las texturas y todas las piezas.


        void restart();                                                              /// Inicializa el estado de gameplay a WAITING.

        void game_over();                                                            /// Establece el estado de gameplay a GAME_OVER.

        void reset_moving_piece();                                                   /// Inicializa la velocidad de moving_piece en el eje que corresponda (el antagónico de current_direction).

        Color next_color();                                                          /// Pasa al siguiente color de la lista.

    private:

        void load ();                                                                /// Carga las texturas par que se vean en la escena.

        void run  (float time);                                                      /// Si está en un estado de "PLAYING", llama al "Update" de la clase "Piece"
    };
}
