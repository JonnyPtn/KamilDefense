/*
	Conrad 'Condzi' Kubacki 2017
	https://github.com/condzi
*/

#pragma once

#include <SFML/Graphics/Sprite.hpp>

#include <Logger.hpp>

#include "BoxCollider.hpp"
#include "Damagable.hpp"
#include "Drawable.hpp"
#include "Entity.hpp"
#include "MissileManager.hpp"

namespace kd
{
	class Enemy :
		public Damagable,
		public Drawable,
		public BoxCollider,
		public Entity
	{
	public:
		Enemy() :
			BoxCollider( this ),
			shootTime( ENEMY_SHOOT_COOLDOWN )
		{}

		void SetTexture( std::weak_ptr<sf::Texture> tex ) override;
		void SetPosition( const sf::Vector2f& pos ) override;

		void Update( seconds_t dt ) override;
		void Draw( sf::RenderTarget& target ) override;

	private:
		sf::Sprite sprite;

		seconds_t shootTime;

		void updateMovement( seconds_t dt );
		void shoot();
	};
}
