/*
	Conrad 'Condzi' Kubacki 2017
	https://github.com/condzi
*/

#include "Background.hpp"

namespace kd
{
	void Background::SetTexture( std::weak_ptr<sf::Texture> tex )
	{
		if ( tex.expired() )
			cgf::Logger::Log( "Background texture is not assigned / is expired", cgf::Logger::ERROR );
		else
		{
			this->sprite.setTexture( *tex.lock() );
			this->sprite.scale( 2.0f, 2.0f );
			this->sprite.scale( SCALE, SCALE );
		}
	}

	void Background::Draw( sf::RenderTarget& target )
	{
		if ( this->sprite.getTexture() == nullptr )
			cgf::Logger::Log( "Background texture is not set, nothing will be drawn", cgf::Logger::WARNING, cgf::Logger::CONSOLE );
		else
			target.draw( this->sprite );
	}
}