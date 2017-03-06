/*
	Conrad 'Condzi' Kubacki 2017
	https://github.com/condzi
*/

#include "Enemy.hpp"

namespace kd
{
	void Enemy::SetTexture( std::shared_ptr<sf::Texture> tex )
	{
		if ( !tex )
			cgf::Logger::Log( "Enemy texture is not assigned", cgf::Logger::WARNING );
		else
		{
			this->texture = tex;

			this->sprite.setTexture( *this->texture.lock() );
			this->sprite.setScale( SCALE, SCALE );

			this->rectangle = this->sprite.getGlobalBounds();
		}
	}

	void Enemy::SetPosition( const sf::Vector2f& pos )
	{
		this->position = pos;
		this->rectangle.left = pos.x;
		this->rectangle.top = pos.y;
	}

	void Enemy::Update( seconds_t dt )
	{
		this->updateMovement( dt );

		this->updateDamage( dt );

		if ( this->health == 0 )
			this->wishDelete = true;

		this->shootTime -= dt;
		if ( this->shootTime <= 0.0f )
			this->shoot();
	}
	void Enemy::Draw( sf::RenderTarget& target )
	{
		if ( this->texture.expired() )
			cgf::Logger::Log( "Enemy texture is not set, nothing will be drawn", cgf::Logger::WARNING, cgf::Logger::CONSOLE );
		else
			target.draw( this->sprite );
	}

	void Enemy::updateMovement( seconds_t dt )
	{
		this->velocity.y += GRAVITY * OBJECT_WEIGHT * dt;

		this->rectangle.left += this->velocity.x * dt;
		this->rectangle.top += this->velocity.y * dt;

		this->position = { this->rectangle.left, this->rectangle.top };

		this->sprite.setPosition( this->position );
	}

	void Enemy::shoot()
	{
		this->shootTime = ENEMY_SHOOT_COOLDOWN;

		auto missileLeft = std::make_shared<Missile>();
		auto missileRight = std::make_shared<Missile>();

		missileLeft->SetType( entityID_t::BULLET_ENEMY );
		missileRight->SetType( entityID_t::BULLET_ENEMY );

		missileLeft->SetPosition( this->position );
		missileRight->SetPosition( { this->position.x + ( 4 * SCALE ), this->position.y } );

		missileLeft->velocity.x = -ENEMY_MISSILE_SPEED;
		missileRight->velocity.x = ENEMY_MISSILE_SPEED;

		missileLeft->rectangle.width = 5.0f;
		missileLeft->rectangle.height = 2.5f;
		missileRight->rectangle.width = missileLeft->rectangle.width;
		missileRight->rectangle.height = missileLeft->rectangle.height;

		MissileManager::AddMissile( missileLeft );
		MissileManager::AddMissile( missileRight );
	}
}