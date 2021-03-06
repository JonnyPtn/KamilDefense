/*
	Conrad 'Condzi' Kubacki 2017
	https://github.com/condzi
*/

#pragma once

#include <Logger.hpp>

#include "BoxCollider.hpp"
#include "Entity.hpp"
#include "Enemy.hpp"
#include "Player.hpp"

namespace kd
{
	class CollisionSolver final
	{
	public:
		CollisionSolver() = delete;

		static void EntityEntity( std::weak_ptr<BoxCollider> colliderA, std::weak_ptr<BoxCollider> colliderB, collisionSide_t colliderAcollisionSide );
		static void EnemyEntity( std::weak_ptr<BoxCollider> enemyCollider, std::weak_ptr<BoxCollider> colliderB, collisionSide_t enemyCollisionSide );
		static void EnemyPlayer( std::weak_ptr<BoxCollider> enemyCollider, std::weak_ptr<BoxCollider> colliderB, collisionSide_t enemyCollisionSide );
		static void BulletEntity( std::weak_ptr<BoxCollider> bulletCollider );
		static void BulletEnemy( std::weak_ptr<BoxCollider> bulletCollider, std::weak_ptr<BoxCollider> colliderB );
		static void BulletPlayer( std::weak_ptr<BoxCollider> bulletCollider, std::weak_ptr<BoxCollider> colliderB );
		static void BulletBullet( std::weak_ptr<BoxCollider> bulletColliderA, std::weak_ptr<BoxCollider> bulletColliderB );
	};
}

