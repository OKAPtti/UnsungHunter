#include "ArsDeathActionState.h"
#include "../../../../GamaObjects/Character/ArsAllCharactersIncludes.h"
#include "../../../../Scene/BaseScene/ArsScene.h"

namespace Alrescha {
	DeathActionState::DeathActionState(void)
		: EnemyState(eDeathAcitionState)
		, mIsProcessed(false)
		, mAnimeFrame(0.0f)
	{}
	DeathActionState::~DeathActionState(void)
	{}
	void DeathActionState::Start(StateMachine* apStateMachine, WPtr<class GameObject> apGameObject, Scene* apScene)
	{
		EnemyState::Start(apStateMachine, apGameObject, apScene);
	}
	void DeathActionState::Update(float aDeltaTime)
	{
		//“|‚ê‚½ŽžA’n–Ê‚É“Y‚í‚·‚½‚ß‚Éˆê“x‚¾‚¯ˆ—‚·‚é.
		if (mIsProcessed) {
			auto pEnemy = mpEnemy.lock();
			if (!pEnemy) { return; }
			const D3DXVECTOR3 UnderDirection(0.0f, -1.0f, 0.0f);
			auto info = mpScene->RayTest(pEnemy->GetPosition(), UnderDirection, eTerrain);
			constexpr float DistanceFromGround = 0.15f;
			if (info.mDistance >= DistanceFromGround && mAnimeFrame >= 0.75f) {
				pEnemy->AddPosition({ 0.0f, (DistanceFromGround - info.mDistance), 0.0f });
			}
			return;
		}

		Animation();
	}
	void DeathActionState::OnChangeEvent(void)
	{
		mIsNext = false;
	}
	void DeathActionState::Animation(void)
	{
		D3DXQUATERNION startQua, endQua, nowQua;

		D3DXMATRIX frontMat, backMat;
		D3DXMatrixIdentity(&frontMat);
		D3DXMatrixIdentity(&backMat);

		constexpr float FrontDeg = 0.0f;
		constexpr float BackDeg = 10.0f;

		D3DXMatrixRotationZ(&frontMat, D3DXToRadian(FrontDeg));
		D3DXMatrixRotationZ(&backMat, D3DXToRadian(BackDeg));

		D3DXQuaternionRotationMatrix(&startQua, &frontMat);
		D3DXQuaternionRotationMatrix(&endQua, &backMat);

		D3DXQuaternionSlerp(&nowQua, &startQua, &endQua, mAnimeFrame);

		constexpr float frameSize = 0.05f;
		mAnimeFrame += frameSize;

		D3DXMATRIX nowMat;
		D3DXMatrixRotationQuaternion(&nowMat, &nowQua);

		if (mAnimeFrame >= 1.0f) {
			mAnimeFrame = 1.0f;
			mIsProcessed = true;
		}

		auto pEnemy = mpEnemy.lock();
		if (!pEnemy) { return; }

		pEnemy->AddRotation(nowMat);
	}
}