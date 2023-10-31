#include <array>
#include <chrono>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <algorithm>

#include "Grados_a_Radianes.hpp"
#include "Tomar_Grados.hpp"
#include "Global.hpp"
#include "Colision_mapa.hpp"
#include "Radianes_a_grados.hpp"
#include "Enemigo.hpp"
#include "Player.hpp"

using namespace std;
using namespace sf;

Player::Player(float i_x, float i_y) :
	movhorizontal(0),
	movvertical(0),
	x(i_x),
	y(i_y),
	map_player_sprite(map_player_texture),
	wall_sprite(wall_texture)
{
	map_player_texture.loadFromFile("sprites/personaje2d" + to_string(TAMANO_CASILLA_EN_MAPA) + ".png");
	enemigo_texture.loadFromFile("sprites/perrito" + to_string(TAMANO_CASILLA) + ".png");
	wall_texture.loadFromFile("sprites/wall" + to_string(TAMANO_CASILLA) + ".png");

	enemigo_sprite.setTexture(enemigo_texture);
}

void Player::draw_map(RenderWindow& i_ventana)
{
	//si tenemos 8 fotogramas, cada uno representara 45 grados
	float angulo_fotgrama = 360.f * TAMANO_CASILLA_EN_MAPA / map_player_texture.getSize().x;
	//necesitamos cambiar los angulos en la mitad �ra que encajen con cada frame
	float direccion_cambiada = tomar_grados(movhorizontal + 0.5f * angulo_fotgrama);
	float start_x = x + 0.5f * TAMANO_CASILLA;
	float start_y = y + 0.5f * TAMANO_CASILLA;

	//Visualizan do el CDV
	VertexArray fov_visualization(TriangleFan, 1 + ANCHO_PANTALLA);
	fov_visualization[0].position = Vector2f(TAMANO_CASILLA_EN_MAPA * start_x / TAMANO_CASILLA, TAMANO_CASILLA_EN_MAPA * start_y / TAMANO_CASILLA);

	map_player_sprite.setPosition(round(TAMANO_CASILLA_EN_MAPA * x / TAMANO_CASILLA), round(TAMANO_CASILLA_EN_MAPA * y / TAMANO_CASILLA));
	map_player_sprite.setTextureRect(IntRect(static_cast<unsigned short>(TAMANO_CASILLA_EN_MAPA * floor(direccion_cambiada / angulo_fotgrama)), 0, TAMANO_CASILLA_EN_MAPA, TAMANO_CASILLA_EN_MAPA));

	for (unsigned short a = 0; a < ANCHO_PANTALLA; a++)
	{
		float direccion_rayos = tomar_grados(movhorizontal + CDV_HORIZONRTAL * (floor(0.5f * ANCHO_PANTALLA) - 1 - a) / (ANCHO_PANTALLA - 1));

		//buscando el final de los rayos con trigonometria
		fov_visualization[1 + a].position = Vector2f(TAMANO_CASILLA_EN_MAPA * (start_x + verrayos[a] * cos(grados_a_radianes(direccion_rayos))) / TAMANO_CASILLA, TAMANO_CASILLA_EN_MAPA * (start_y - verrayos[a] * sin(grados_a_radianes(direccion_rayos))) / TAMANO_CASILLA);
	}

	i_ventana.draw(fov_visualization);
	i_ventana.draw(map_player_sprite);
}


void Player::bibujar_ventana(RenderWindow& i_ventana, const Enemigo& i_enemigo)
{
	bool draw_enemigo = 0;

	//esta es la distacian ciando la altura de la pryeccion y el ancho de la pared en frente de nosotros sea igual.
	float distancia_de_proyeccion = 0.5f * TAMANO_CASILLA / tan(grados_a_radianes(0.5f * CDV_VERTICAL));
	//esto definira el piso
	float nivel_del_suelo = round(0.5f * ALTURA_PANTALLA * (1 + tan(grados_a_radianes(movvertical)) / tan(grados_a_radianes(0.5f * CDV_VERTICAL))));
	float rayo_start_x = x + 0.5f * TAMANO_CASILLA;
	float rayo_start_y = y + 0.5f * TAMANO_CASILLA;
	//estos seran los angulos absolutos entre el enemigo y el jugador
	//segun wikipedia
	//a funcion atan2 se define como los angulos en el plano Euclideano, dado en radianes, entre los ejes positivos de x y el rayo al punto (x , y) =/= (0 , 0)
	float direccion_enemigo = tomar_grados(radianes_a_grados(atan2(rayo_start_x - i_enemigo.get_centro_de_y(), i_enemigo.get_centro_de_x() - rayo_start_x))) - movhorizontal;
	//aplicamos pitagoras
	float distancia_enemigo = static_cast<float>(sqrt(pow(rayo_start_x - i_enemigo.get_centro_de_x(), 2) + pow(rayo_start_y - i_enemigo.get_centro_de_y(), 2)));
	//la posicion de las columnas no puede ser negativo, asi que usamos SHRT_MIN
	short columna_previa = SHRT_MIN;

	RectangleShape floor_shape(Vector2f(ANCHO_PANTALLA, ALTURA_PANTALLA - nivel_del_suelo));
	floor_shape.setFillColor(Color(0, 0, 0));
	floor_shape.setPosition(0, nivel_del_suelo);

	//esto se asegurara de que el angulo este entre -180 y 180 grados
	if (-180 >= direccion_enemigo)
	{
		direccion_enemigo += 360;
	}
	else if (180 < direccion_enemigo)
	{
		direccion_enemigo -= 360;
	}

	//si estamos lejos de enemigo o el esta detra nuestro entonces debe no debe ser dibujado
	draw_enemigo = DISTANCIA_DE_RENDER >= distancia_enemigo && direccion_enemigo <= 0.75f * CDV_HORIZONRTAL && direccion_enemigo >= -0.75f * CDV_HORIZONRTAL;

	i_ventana.draw(floor_shape);

	for (unsigned short a = 0; a < ANCHO_PANTALLA; a++)
	{
		//dibujamos las columnas que estan detras del enemigo
		if (0 == (1 == draw_enemigo && distancia_enemigo > verrayos[a]))
		{
			/*
			cuando "a" is 0, divimos el +CDV (campo de vision) / 2
			cuando "a" is ALTURA_PANTALLA / 2, esto sera 0
			cuando "a" is ANCHO_PANTALLA, esto sera -CDV / 2
			*/
			float direccion_rayo = CDV_HORIZONRTAL * (floor(0.5f * ANCHO_PANTALLA) - a) / (ANCHO_PANTALLA - 1);
			//buscamos la interseccion entre el rayo y la proyeccion
			float posicion_del_rayo_proyectado = 0.5f * tan(grados_a_radianes(direccion_rayo)) / tan(grados_a_radianes(0.5f * CDV_HORIZONRTAL));

			//posicion de la columna actual en pantalla
			short columna_actual = static_cast<short>(round(ANCHO_PANTALLA * (0.5f - posicion_del_rayo_proyectado)));
			short siguiente_columna = ANCHO_PANTALLA;

			if ( a < ANCHO_PANTALLA -1)
			{
				float direccion_del_siguente_rayo = CDV_HORIZONRTAL * (floor(0.5f * ANCHO_PANTALLA) - 1 - a) / (ANCHO_PANTALLA - 1);

				posicion_del_rayo_proyectado = 0.5 * tan(grados_a_radianes(direccion_del_siguente_rayo)) / tan(grados_a_radianes(0.5f * CDV_HORIZONRTAL));

				siguiente_columna = static_cast<short>(round(ANCHO_PANTALLA * (0.5f - posicion_del_rayo_proyectado)));
			}

			//esto prevendra que se dibuje una columna encima de la otra
			if (columna_previa < columna_actual)
			{
				float rayo_que_acaba_en_x = rayo_start_x + verrayos[a] * cos(grados_a_radianes(tomar_grados(movhorizontal + direccion_rayo)));
				float rayo_que_acaba_en_y = rayo_start_y - verrayos[a] * sin(grados_a_radianes(tomar_grados(movhorizontal + direccion_rayo)));
				//esto sera la posicion de la textura de la columna de la pared que se este dibujando
				float textura_de_la_pared_en_x = 0;

				//para evitar el homa durante el renderizado del juego en la DISTANCIA_DE_RENDER / 2 o mas cerca de nosotros
				//de lo contrario debemos estar a 255 pixeles en las DISTANCIA_DE_RENDER o mas lejos
				//255-0 en la DISTANCIA_DE_RENDER y la DISTANCIA_DE_RENDER / 2
				//0 an la DISTANCIA_DE_rENDER / 2 onmas cercano
				unsigned char brillo = static_cast<unsigned char>(round(255 * max<float>(0, 2 * verrayos[a] / DISTANCIA_DE_RENDER - 1)));

				//usamos coseno para prevenir el efecto de ojo de pescado (es decir cuando de acercas auna pared esta de pone curva)
				unsigned short altura_de_la_columna = static_cast<unsigned short>(ALTURA_PANTALLA * distancia_de_proyeccion / (verrayos[a] * cos(grados_a_radianes(direccion_rayo))));

				//aqui esta nuestro humo, lo dibujaremos como una columna transparente con el color del cielo
				RectangleShape shape(Vector2f(max(1, siguiente_columna - columna_actual), altura_de_la_columna));
				shape.setFillColor(Color(255, 0, 255, brillo));
				shape.setPosition(columna_actual, round(nivel_del_suelo - 0.5 * altura_de_la_columna));

				columna_previa = columna_actual;

				//checando se en choque del rayo es vertical u horzontal
				if (abs(rayo_que_acaba_en_x - TAMANO_CASILLA * round(rayo_que_acaba_en_y / TAMANO_CASILLA)) < abs(rayo_que_acaba_en_y - TAMANO_CASILLA * round(rayo_que_acaba_en_y / TAMANO_CASILLA)))
				{
					textura_de_la_pared_en_x = rayo_que_acaba_en_y - TAMANO_CASILLA * floor(rayo_que_acaba_en_y / TAMANO_CASILLA);
				}
				else
				{
					textura_de_la_pared_en_x = TAMANO_CASILLA * ceil(rayo_que_acaba_en_x / TAMANO_CASILLA) - rayo_que_acaba_en_x;
				}

				wall_sprite.setPosition(columna_actual, round(nivel_del_suelo - 0.5f * altura_de_la_columna));
				wall_sprite.setTextureRect(IntRect(static_cast<unsigned short>(round(textura_de_la_pared_en_x)), 0, 1, TAMANO_CASILLA));
				wall_sprite.setScale(max(1, siguiente_columna - columna_actual), altura_de_la_columna / static_cast<float>(TAMANO_CASILLA));
			
				i_ventana.draw(wall_sprite);
				i_ventana.draw(shape);
			}
		}
	}

	if (1 == draw_enemigo)
	{
		float angulo_fotograma = 360.f * TAMANO_CASILLA / enemigo_texture.getSize().x;
		//tomando de duireccion relative del enemigo hacia nsotros
		float direccion_cambiada = tomar_grados(i_enemigo.Get_direccion() + 0.5f * (180 * angulo_fotograma) - movhorizontal - direccion_enemigo);
		float posicion_del_enemigo_proyectado = 0.5f * tan(grados_a_radianes(direccion_enemigo)) / tan(grados_a_radianes(0.5f * CDV_HORIZONRTAL));

		short enemigo_en_pantalla_x = static_cast<short>(round(ANCHO_PANTALLA * (0.5f - posicion_del_enemigo_proyectado)));

		unsigned short tamano_del_enemigo = static_cast<unsigned short>(ALTURA_PANTALLA * distancia_de_proyeccion / (distancia_enemigo * cos(grados_a_radianes(direccion_enemigo))));

		columna_previa = SHRT_MIN;

		enemigo_sprite.setColor(Color(255 , 255 , 255, static_cast<unsigned char>(round(255 * min<float>(1 , 2 * (1 - distancia_enemigo / DISTANCIA_DE_RENDER))))));
		enemigo_sprite.setPosition(round(enemigo_en_pantalla_x - 0.5f * tamano_del_enemigo), round (nivel_del_suelo - 0.5f * tamano_del_enemigo));
		enemigo_sprite.setScale(tamano_del_enemigo / static_cast<float>(TAMANO_CASILLA), tamano_del_enemigo / static_cast<float>(TAMANO_CASILLA));
		enemigo_sprite.setTextureRect(IntRect(static_cast<unsigned short>(TAMANO_CASILLA * floor(direccion_cambiada / angulo_fotograma)), 0, TAMANO_CASILLA, TAMANO_CASILLA));

		i_ventana.draw(enemigo_sprite);

		for (unsigned short a = 0; a < ANCHO_PANTALLA; a++)
		{
			//dibujamos las columnas que estaras cerca del enemigo
			if (distancia_enemigo > verrayos[a])
			{
				float direccion_de_los_rayos = CDV_HORIZONRTAL * (floor(0.5f * ANCHO_PANTALLA) - a) / (ANCHO_PANTALLA - 1);
				float posicion_de_los_rayos_proyectados = 0.5f * tan(grados_a_radianes(direccion_de_los_rayos)) / tan(grados_a_radianes(0.5f * CDV_HORIZONRTAL));

				short current_column = static_cast<short>(round(ANCHO_PANTALLA * (0.5f - posicion_de_los_rayos_proyectados)));
				short siguiente_columna = ANCHO_PANTALLA;

				if (a < ANCHO_PANTALLA - 1)
				{
					float direccion_del_siguiente_rayo = CDV_HORIZONRTAL * (floor(0.5f * ANCHO_PANTALLA) - 1 - a) / (ANCHO_PANTALLA - 1);

					posicion_de_los_rayos_proyectados = 0.5f * tan(grados_a_radianes(direccion_del_siguiente_rayo)) / tan(grados_a_radianes(0.5f * CDV_HORIZONRTAL));

					siguiente_columna = static_cast<short>(round(ANCHO_PANTALLA * (0.5f - posicion_de_los_rayos_proyectados)));
				}

				if (columna_previa < current_column)
				{
					float rayo_que_acaba_en_x = rayo_start_x + verrayos[a] * cos(grados_a_radianes(tomar_grados(movhorizontal + direccion_de_los_rayos)));
					float rayo_que_acaba_en_y = rayo_start_y - verrayos[a] * sin(grados_a_radianes(tomar_grados(movhorizontal + direccion_de_los_rayos)));
					float textura_de_la_pared_x = 0;

					unsigned char brillo = static_cast<unsigned char>(round(255 * max<float>(0, 2 * verrayos[a] / DISTANCIA_DE_RENDER - 1)));

					unsigned short altura_de_la_columna = static_cast<unsigned short>(ALTURA_PANTALLA * distancia_de_proyeccion / (verrayos[a] * cos(grados_a_radianes(direccion_de_los_rayos))));

					RectangleShape shape(Vector2f(max(1, siguiente_columna - current_column), altura_de_la_columna));
					shape.setFillColor(Color(255, 0, 0, brillo));
					shape.setPosition(current_column, round(nivel_del_suelo - 0.5 * altura_de_la_columna));

					columna_previa = current_column;

					if (abs(rayo_que_acaba_en_x - TAMANO_CASILLA * round(rayo_que_acaba_en_x / TAMANO_CASILLA)) < abs(rayo_que_acaba_en_y - TAMANO_CASILLA * round(rayo_que_acaba_en_y / TAMANO_CASILLA)))
					{
						textura_de_la_pared_x = rayo_que_acaba_en_y - TAMANO_CASILLA * floor(rayo_que_acaba_en_y / TAMANO_CASILLA);
					}
					else
					{
						textura_de_la_pared_x = TAMANO_CASILLA * ceil(rayo_que_acaba_en_x / TAMANO_CASILLA) - rayo_que_acaba_en_x;
					}

					wall_sprite.setPosition(current_column, round(nivel_del_suelo - 0.5f * altura_de_la_columna));
					wall_sprite.setTextureRect(IntRect(static_cast<unsigned short>(round(textura_de_la_pared_x)), 0, 1, TAMANO_CASILLA));
					wall_sprite.setScale(max(1, siguiente_columna - current_column), altura_de_la_columna / static_cast<float>(TAMANO_CASILLA));

					i_ventana.draw(wall_sprite);
					i_ventana.draw(shape);
				}
			}
		}
	}
}

void Player::set_position(float i_x, float i_y)
{
	x = i_x;
	y = i_y;
}

void Player::update(const array<array<Cell, ALTURA_MAPA>, ANCHO_MAPA>& i_map, const RenderWindow& i_ventana)
{
	float rotacion_horizontal = 0;
	float rotacion_vertical = 0;
	float paso_x = 0;
	float paso_y = 0;

	unsigned short centrar_ventana_en_x = static_cast<unsigned short>(round(0.5f * i_ventana.getSize().x));
	unsigned short centrar_ventana_en_y = static_cast<unsigned short>(round(0.5f * i_ventana.getSize().y));

	//hacemos en controlador del mouse
	rotacion_horizontal = CDV_HORIZONRTAL * (centrar_ventana_en_x - Mouse::getPosition(i_ventana).x) / i_ventana.getSize().x;
	rotacion_vertical = CDV_VERTICAL * (centrar_ventana_en_y - Mouse::getPosition(i_ventana).y) / i_ventana.getSize().y;

	movhorizontal = tomar_grados(movhorizontal + rotacion_horizontal);
	//el programa crashea con una vista en 90 grados asi que use 89
	movvertical = clamp<float>(movvertical + rotacion_vertical, -89, 89);

	//esto servira para que la pantalla no se salga de foco
	Mouse::setPosition(Vector2i(centrar_ventana_en_x, centrar_ventana_en_y), i_ventana);
	
	if (1 == Keyboard::isKeyPressed(Keyboard::A))
	{
		paso_x = VEL_MOV * cos(grados_a_radianes(tomar_grados(90 + movhorizontal)));
		paso_y = -VEL_MOV * sin(grados_a_radianes(tomar_grados(90 + movhorizontal)));
	}
	else if (1 == Keyboard::isKeyPressed(Keyboard::D))
	{
		paso_x = VEL_MOV * cos(grados_a_radianes(tomar_grados(movhorizontal - 90)));
		paso_y = -VEL_MOV * sin(grados_a_radianes(tomar_grados(movhorizontal - 90)));
	}
	else if (1 == Keyboard::isKeyPressed(Keyboard::S))
	{
		paso_x -= VEL_MOV * cos(grados_a_radianes(movhorizontal));
		paso_y += -VEL_MOV * sin(grados_a_radianes(movhorizontal));
	}
	else if (1 == Keyboard::isKeyPressed(Keyboard::W))
	{
		paso_x += VEL_MOV * cos(grados_a_radianes(movhorizontal));
		paso_y -= -VEL_MOV * sin(grados_a_radianes(movhorizontal));
	}

	if (0 == colision_mapa(paso_x + x, paso_y + y, i_map))
	{
		x += paso_x;
		y += paso_y;
	}
	else if (0 == colision_mapa(paso_x + x, y, i_map))
	{
		x += paso_x;
		y = TAMANO_CASILLA * round(y / TAMANO_CASILLA);
	}
	else if (0 == colision_mapa(x, paso_y + y, i_map))
	{
		x = TAMANO_CASILLA * round(x / TAMANO_CASILLA);
		y += paso_y;
	}
	else
	{
		//ponemos al jugador en la celda mas cercana entes de chocar con la pared
		x = TAMANO_CASILLA * round(x / TAMANO_CASILLA);
		y = TAMANO_CASILLA + round(y / TAMANO_CASILLA);
	}

	for (unsigned short a = 0; a < ANCHO_PANTALLA; a++)
	{
		char paso_x_por_casilla = 0;
		char paso_y_por_casilla = 0;

		float direccion_del_rayo = tomar_grados(movhorizontal + CDV_HORIZONRTAL * (floor(0.5f * ANCHO_PANTALLA) - a) / (ANCHO_PANTALLA - 1));
		float direccion_del_rayo_x = cos(grados_a_radianes(direccion_del_rayo));
		float direccion_del_rayo_y = -sin(grados_a_radianes(direccion_del_rayo));

		float largo_del_rayo = 0;
		float rayo_que_empieza_en_x = x + 0.5f * TAMANO_CASILLA;
		float rayo_que_empieza_en_y = y + 0.5f * TAMANO_CASILLA;
		//este rayo chequea las colisiones horizontales
		float largo_del_rayo_x = 0;
		//este rayo chequea las colisiones verticales
		float largo_del_rayo_y = 0;
		//este sera el largo del rayo al moverse 1 unidad a lo largo del eje x
		float largo_del_rayo_x_por_unidad = static_cast<float>(TAMANO_CASILLA * sqrt(1 + pow(direccion_del_rayo_y / direccion_del_rayo_x, 2)));
		//este sera el largo del rayo al moverse 1 unidad a lo largo del eje y
		float largo_del_rayo_y_por_unidad = static_cast<float>(TAMANO_CASILLA * sqrt(1 + pow(direccion_del_rayo_x / direccion_del_rayo_y, 2)));
		
		unsigned char casilla_actual_x = static_cast<unsigned char>(floor(rayo_que_empieza_en_x / TAMANO_CASILLA));
		unsigned char casilla_actual_y = static_cast<unsigned char>(floor(rayo_que_empieza_en_y / TAMANO_CASILLA));

		if (0 > direccion_del_rayo_x)
		{
			paso_x_por_casilla = -1;

			//los rayos deben iniciar a los bordes de la casilla
			//si es que los rayos no se setean al borde la casilla, se tendra que aproxo�imarlo al mas cercano
			largo_del_rayo_x = largo_del_rayo_x_por_unidad * (rayo_que_empieza_en_x / TAMANO_CASILLA - casilla_actual_x);
		}
		else if (0 < direccion_del_rayo_x)
		{
			paso_x_por_casilla = 1;

			largo_del_rayo_x = largo_del_rayo_x_por_unidad * (1 + casilla_actual_x - rayo_que_empieza_en_x / TAMANO_CASILLA);
		}
		else
		{
			paso_x_por_casilla = 0;
		}

		if (0 > direccion_del_rayo_y)
		{
			paso_y_por_casilla = -1;

			largo_del_rayo_y = largo_del_rayo_y_por_unidad * (rayo_que_empieza_en_y / TAMANO_CASILLA - casilla_actual_y);
		}
		else if (0 < direccion_del_rayo_y)
		{
			paso_y_por_casilla = 1;

			largo_del_rayo_y = largo_del_rayo_y_por_unidad * (1 + casilla_actual_y - rayo_que_empieza_en_y / TAMANO_CASILLA);
		}
		else
		{
			paso_y_por_casilla = 0;
		}

		//continuamos con la trasmision del rayo hasta lograr la distancia de renderizado
		while (DISTANCIA_DE_RENDER >= largo_del_rayo)
		{
			//en caso de que el rayo choque con una esquina
			bool colision_esquina = 0;

			//estiramos el rayo mas corto
			if (largo_del_rayo_x < largo_del_rayo_y)
			{
				largo_del_rayo = largo_del_rayo_x;
				largo_del_rayo_x += largo_del_rayo_x_por_unidad;

				casilla_actual_x +=  paso_x_por_casilla;
			}
			else if (largo_del_rayo_x > largo_del_rayo_y)
			{
				largo_del_rayo = largo_del_rayo_y;
				largo_del_rayo_y += largo_del_rayo_y_por_unidad;

				casilla_actual_y += paso_y_por_casilla;
			}
			else
			{
				//si los rayos son iguales significa que chocamos con la esquina, asi que estiramos ambos rayos
				colision_esquina = 1;

				largo_del_rayo = largo_del_rayo_x;
				largo_del_rayo_x += largo_del_rayo_x_por_unidad;
				largo_del_rayo_y += largo_del_rayo_y_por_unidad;

				casilla_actual_x += paso_x_por_casilla;
				casilla_actual_y += paso_y_por_casilla;
			}

			//asegurandonos que la casilla ctual que estemos cheqeuadno este dentro del mapa
			if (0 <= casilla_actual_x && 0 <= casilla_actual_y && ALTURA_MAPA > casilla_actual_y && ANCHO_MAPA > casilla_actual_x)
			{
				if (Cell::Wall == i_map[casilla_actual_x][casilla_actual_y])
				{
					//detenemos la transmision de los rayos cuando choqiemn una pared
					break;
				}
				else if (1 == colision_esquina)
				{
					//el rayo no puede pasar a traves de dos paredes que permanecen diagonales
					if (Cell::Wall == i_map[casilla_actual_x - paso_x_por_casilla][casilla_actual_y] && Cell::Wall == i_map[casilla_actual_x][casilla_actual_y - paso_y_por_casilla])
					{
						break;
					}
				}
			}
		}

		//el largo de los rayos debe ser menor o igual a la distancia de renderizado
		largo_del_rayo = min(DISTANCIA_DE_RENDER, largo_del_rayo);

		verrayos[a] = largo_del_rayo;
	}
}