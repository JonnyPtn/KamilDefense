/*
	Conrad 'Condzi' Kubacki 2017
	https://github.com/condzi
*/

#include "Level.hpp"

namespace kd
{
	bool Level::Load( const std::string& path )
	{
		cgf::Logger::Log( "Level loading started..." );

		std::ifstream levelFile( path, std::ios::binary );
		if ( !levelFile.good() )
		{
			cgf::Logger::Log( "Level loading error - cannot open file", cgf::Logger::ERROR );
			return false;
		}

		size_t bordersSznew = 0;
		size_t spawnersSznew = 0;
		size_t strSznew = 0;

		// loading new size for borders and spawners vector
		levelFile.read( reinterpret_cast<char*>( &bordersSznew ), sizeof( bordersSznew ) );
		levelFile.read( reinterpret_cast<char*>( &spawnersSznew ), sizeof( spawnersSznew ) );
		// applying new size
		this->levelData.bordersRects.resize( bordersSznew );
		this->levelData.spawnersData.resize( spawnersSznew );

		// loading bg path size and data
		levelFile.read( reinterpret_cast<char*>( &strSznew ), sizeof( strSznew ) );
		this->levelData.backgroundTexturePath.resize( strSznew );
		levelFile.read( &levelData.backgroundTexturePath[0], strSznew );

		// loading borders and spawners vectors data
		levelFile.read( reinterpret_cast<char*>( &this->levelData.bordersRects[0] ), bordersSznew * sizeof( this->levelData.bordersRects[0] ) );
		levelFile.read( reinterpret_cast<char*>( &this->levelData.spawnersData[0] ), spawnersSznew * sizeof( this->levelData.spawnersData[0] ) );

		// loading player spawn position and bg scale
		levelFile.read( reinterpret_cast<char*>( &this->levelData.playerSpawnPosition ), sizeof( this->levelData.playerSpawnPosition ) );
		levelFile.read( reinterpret_cast<char*>( &this->levelData.backgroundTextureScale ), sizeof( this->levelData.backgroundTextureScale ) );

		cgf::Logger::Log( "Level loading finished" );

		this->checkLevelData();
		this->initializeEntities();

		return true;
	}

	void Level::Save( const std::string& path )
	{
		cgf::Logger::Log( "Level saving started..." );

		std::ofstream levelFile( path, std::ios::binary | std::ios::trunc );
		// Saving vectors sizes to file
		size_t bordersSz = this->levelData.bordersRects.size();
		size_t spawnersSz = this->levelData.spawnersData.size();
		size_t strSz = this->levelData.backgroundTexturePath.size();

		levelFile.write( reinterpret_cast<const char*>( &bordersSz ), sizeof( bordersSz ) );
		levelFile.write( reinterpret_cast<const char*>( &spawnersSz ), sizeof( spawnersSz ) );
		// Saving bg path size and data
		levelFile.write( reinterpret_cast<const char*>( &strSz ), sizeof( strSz ) );
		levelFile.write( &this->levelData.backgroundTexturePath[0], strSz );
		// saving std::vectors data
		levelFile.write( reinterpret_cast<const char*>( &this->levelData.bordersRects[0] ), bordersSz * sizeof( this->levelData.bordersRects[0] ) );
		levelFile.write( reinterpret_cast<const char*>( &this->levelData.spawnersData[0] ), spawnersSz * sizeof( this->levelData.spawnersData[0] ) );

		// saving playerSpawnPosition and bgScale
		levelFile.write( reinterpret_cast<const char*>( &this->levelData.playerSpawnPosition ), sizeof( this->levelData.playerSpawnPosition ) );
		levelFile.write( reinterpret_cast<const char*>( &this->levelData.backgroundTextureScale ), sizeof( this->levelData.backgroundTextureScale ) );

		cgf::Logger::Log( "Level saving finished" );
	}

	void Level::InitializeTextures()
	{
		ResourceHolder::DeleteAllResourcesByPriority( static_cast<uint8_t>( resourcePriorites_t::LEVEL ) );

		ResourceHolder::textures.push_back( std::make_shared<textureResource_t>() );

		if ( !ResourceHolder::textures.back()->loadFromFile( this->levelData.backgroundTexturePath ) )
		{
			ResourceHolder::textures.pop_back();
			cgf::Logger::Log( "Cannot load Level Background Texture \"" + this->levelData.backgroundTexturePath + "\"", cgf::Logger::ERROR );
		} else
		{
			ResourceHolder::textures.back()->SetResourceID( static_cast<uint8_t>( textureResourceID_t::LEVEL_BG ) );
			ResourceHolder::textures.back()->SetResourcePriority( static_cast<uint8_t>( resourcePriorites_t::LEVEL ) );
			this->background->SetTexture( ResourceHolder::textures.back() );
			this->background->SetSpriteScale( this->levelData.backgroundTextureScale );
		}

		for(auto spawner : this->spawners)
			spawner->SetEnemyTexture( ResourceHolder::GetTexture( static_cast<uint8_t>( textureResourceID_t::ENEMY ) ) );
	}

	void Level::SetPlayerPosition( Player* player )
	{
		player->SetPosition( this->levelData.playerSpawnPosition );
	}

	void Level::AddEntities( std::vector<std::shared_ptr<Entity>>* entitiesPtr, CollisionChecker* collisionCheckerPtr )
	{
		for ( auto border : this->borders )
			entitiesPtr->push_back( border );

		for ( auto spawner : this->spawners )
		{
			spawner->SetPhysicChecker( collisionCheckerPtr );
			spawner->SetEntitiesVector( entitiesPtr );
			entitiesPtr->push_back( spawner );
		}

		entitiesPtr->push_back( this->background );
		this->background->SetDrawLayer( 0 );

		for ( auto border : this->borders )
			collisionCheckerPtr->AddBoxCollider( border );
	}

	void Level::RemoveEntities()
	{
		for ( auto border : this->borders )
			border->SetWishDelete( true );

		for ( auto spawner : this->spawners )
			spawner->SetWishDelete( true );

		if ( this->background )
			this->background->SetWishDelete( true );

		this->borders.clear();
		this->spawners.clear();
		this->background.reset();
	}

	void Level::checkLevelData()
	{
		if ( this->levelData.playerSpawnPosition == sf::Vector2f( 0.0f, 0.0f ) )
			cgf::Logger::Log( "LevelData player spawn position is x(0) y(0) - possible error", cgf::Logger::WARNING );

		if ( this->levelData.bordersRects.size() == 0 )
			cgf::Logger::Log( "LevelData has no border collisions rects - possible error", cgf::Logger::WARNING );

		if ( this->levelData.spawnersData.size() == 0 )
			cgf::Logger::Log( "LevelData has no spawner data - possible error", cgf::Logger::WARNING );

		if ( this->levelData.backgroundTexturePath.size() <= 3 )
			cgf::Logger::Log( "Level data has short background texture path ('" + this->levelData.backgroundTexturePath + "') - possible error", cgf::Logger::WARNING );
	}

	void Level::initializeEntities()
	{
		this->RemoveEntities();

		for ( auto& border : this->levelData.bordersRects )
		{
			this->borders.push_back( std::make_shared<Border>() );
			this->borders.back()->SetType( entityID_t::BORDER );
			this->borders.back()->rectangle = border;
		}

		for ( auto& spawnerData : this->levelData.spawnersData )
		{
			this->spawners.push_back( std::make_shared<EnemySpawner>() );
			this->spawners.back()->SetType( entityID_t::ENEMY_SPAWNER );
			this->spawners.back()->SetSpawningTime( spawnerData.spawnTime );
			this->spawners.back()->SetStartVelocity( spawnerData.velocityDirection );
			this->spawners.back()->SetPosition( spawnerData.position );
		}

		this->background = std::make_shared<Background>();
	}
}