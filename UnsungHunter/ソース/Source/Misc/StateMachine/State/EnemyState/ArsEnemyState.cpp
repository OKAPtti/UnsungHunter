#include "ArsEnemyState.h"
#include "../../../../GamaObjects/Character/ArsAllCharactersIncludes.h"

namespace Alrescha {
	EnemyState::EnemyState(int aType)
		: State(aType)
		, mpEnemy()
	{}
	EnemyState::~EnemyState(void)
	{
	}
	void EnemyState::Start(StateMachine* apStateMachine, WPtr<class GameObject> apGameObject, Scene* apScene)
	{
		State::Start(apStateMachine, apGameObject, apScene);

		auto pEnemy = mpGameObject.lock()->Cast<Enemy>(eWolf);
		if (pEnemy) { mpEnemy = pEnemy; return; }

		auto pBoss = mpGameObject.lock()->Cast<Enemy>(eBoss);
		if (pBoss) { mpEnemy = pBoss; }
		if (pBoss->GetIsHit()) { mpStateMachine->SetIsForciblyChange(); }
	}
}