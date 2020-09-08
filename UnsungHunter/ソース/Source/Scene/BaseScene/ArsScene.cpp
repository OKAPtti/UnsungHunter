#include "ArsScene.h"
#include "../ArsSceneType.h"
#include "../../GamaObjects/BaseGameObjects/ArsGameObject.h"
#include "../../GamaObjects/ArsAllGameObjectIncludes.h"
#include "../../GamaObjects/ArsGameObjectFactory.h"
#include "../ArsSceneManager.h"
#include "../../Core/ArsIncludeCores.h"

namespace Alrescha {

	Scene::Scene(GameFrame* apGameFrame)
		: mpGameFrame(apGameFrame)
		, mGameObjectList(10)
		, mType(SceneType::eScene)
		, mpCamera(std::make_unique<CameraSystem>())
		, mTargetCamera()
	{}

	Scene::~Scene(void)
	{}

	void Scene::InitializeScene(const std::string& aJsonFileName)
	{
		const auto Json = LoadJson(aJsonFileName);
		const auto GameObjectList = Json["GameObjectList"].array_items();

		//数値はdouble型なので、float型にキャストするために一時格納.
		json11::Json::array jsonAry;

		for (const auto& it : GameObjectList) {
			SPtr<GameObject> pObject = nullptr;
			if (it["ClassName"].string_value() == "GameObject") {
				//クラス名がGameObjectの場合はタイプも渡してあげる.
				pObject = CreateGameObject(it["ClassName"].string_value(), it["Type"].int_value());
			}
			else {
				pObject = CreateGameObject(it["ClassName"].string_value());
			}

			if (!pObject) { continue; }

			if (it["DrawOrder"].is_null()) { AddGameObject(pObject); }
			else { AddGameObject(pObject, CastLong(it["DrawOrder"].int_value())); }

			//座標.
			jsonAry = it["Position"].array_items();
			if (!jsonAry.empty()) {
				pObject->SetPosition({
						CastFloat(jsonAry[0].number_value()),
						CastFloat(jsonAry[1].number_value()),
						CastFloat(jsonAry[2].number_value()),
					});
			}

			//回転.
			jsonAry = it["Rotation"].array_items();
			if (!jsonAry.empty()) {
				pObject->SetRotation({
						CastFloat(jsonAry[0].number_value()),
						CastFloat(jsonAry[1].number_value()),
						CastFloat(jsonAry[2].number_value()),
					});
			}

			//拡縮.
			jsonAry = it["Scale"].array_items();
			if (!jsonAry.empty()) {
				pObject->SetScale({
						CastFloat(jsonAry[0].number_value()),
						CastFloat(jsonAry[1].number_value()),
						CastFloat(jsonAry[2].number_value()),
					});
			}

			//描画するか否か(JsonファイルにVisibleが書いてあればis_bool()の戻り値はtrue).
			if (it["Visible"].is_bool()) { pObject->SetIsVisible(false); }

			pObject->Initialize();
			pObject->Deserialize(it);
		}
	}

	void Scene::Update(void)
	{
		GetCamera()->UpdateRotation(GetInput().GetMouseVelocity());

		//ビュー行列更新.
		GetCamera()->UpdateViewMatrix();

		//デルタタイム取得.
		const float DeltaTime = GetDeltaTime();

		for (auto& vectorIt : mGameObjectList) {
			for (auto listIt = vectorIt.begin(); listIt != vectorIt.end();) {
				if ((*listIt)->GetIsAlive()) {
					(*listIt)->Update(DeltaTime);
					++listIt;
				}
				else {
					//KillされたGameObjectをリストから除外
					listIt = vectorIt.erase(listIt);
				}
			}
		}

		//ビュー行列更新.
		GetCamera()->UpdateViewMatrix();

		//タイトルシーンであればゲーム終了できることをGameFrameに伝える.
		if (mType == SceneType::eTitleScene) {
			mpGameFrame->CanFinishGame();
		}
		else{
			mpGameFrame->CanNotFinishGame();
		}

		//BGMを鳴らす.
		GetSoundManager().AddLoopSound("Bgm");
	}

	void Scene::Draw()
	{
		//カメラ設定.
		auto pCamera = mTargetCamera.lock();
		if (pCamera) {
			Get3D().SetCamera(*mpCamera.get());
		}

		//2D(スプライト)描画.
		auto& pSprite = Get3D().GetSprite();
		pSprite.Begin();

		for (auto&& vectorIt : mGameObjectList) {
			for (auto&& listIt : vectorIt) {
				if (listIt->GetIsVisible() &&
					listIt->GetIsAlive()) {
					listIt->Draw2D();
				}
			}
		}

		pSprite.End();

		//3D描画.
		for (auto&& vectorIt : mGameObjectList) {
			for (auto&& listIt : vectorIt) {
				if (listIt->GetIsVisible() &&
					listIt->GetIsAlive()) {
					listIt->Draw3D();
				}
			}
		}
	}
	void Scene::AddGameObject(SPtr<GameObject> apGameObject, UINT aDrawOrder)
	{
		if (aDrawOrder >= 10) {
			_ASSERT_EXPR(false, "10以上の値を渡さないでください");
		}
		apGameObject->SetScene(this);
		mGameObjectList[aDrawOrder].push_back(apGameObject);
	}

	std::list<SPtr<GameObject>> Scene::FindGameObjectList(int aType)
	{
		std::list<SPtr<GameObject>> GameObjectList;
		for (auto& vectorIt : mGameObjectList) {
			for (auto& listIt : vectorIt) {
				if (listIt->GetType() == aType) { GameObjectList.push_back(listIt); }
			}
		}
		return GameObjectList;
	}

	SPtr<GameObject> Scene::FindGameObject(int aType)
	{
		SPtr<GameObject> GameObject;
		for (auto& vectorIt : mGameObjectList) {
			for (auto& listIt : vectorIt) {
				if (listIt->GetType() == aType) { GameObject = listIt; }
			}
		}
		return GameObject;
	}

	std::list<SPtr<GameObject>> Scene::FindGameObjectList(int aBeginType, int aEndType)
	{
		std::list<SPtr<GameObject>> GameObjectList;
		for (auto& vectorIt : mGameObjectList) {
			for (auto& listIt : vectorIt) {
				if (aBeginType <= listIt->GetType() && listIt->GetType() <= aEndType) {
					GameObjectList.push_back(listIt);
				}
			}
		}
		return GameObjectList;
	}
	BOOL Scene::Intersect(LPD3DXMESH& aMesh, const D3DXMATRIX& aAnotherMatrix, const D3DXVECTOR3& aRayShotPos, const D3DXVECTOR3& aRayDirection, float* aDistance)
	{
		D3DXMATRIX invMat;
		D3DXMatrixInverse(&invMat, nullptr, &aAnotherMatrix);

		D3DXVECTOR3 localPos, localVec;
		D3DXVec3TransformCoord(&localPos, &aRayShotPos, &invMat);

		D3DXVec3TransformNormal(&localVec, &aRayDirection, &invMat);

		BOOL hit;

		D3DXIntersect(aMesh, &localPos, &localVec, &hit, nullptr, nullptr, nullptr, aDistance, nullptr, nullptr);
		return hit;
	}

	std::list<RayInfo> Scene::CameraRayTestList(void)
	{
		std::list<SPtr<GameObject>> pGameObjectList = FindGameObjectList(eGameObject3DBegin, eGameObject3DEnd);

		//カメラの逆行列.
		const auto& InverseViewMat = *D3DXMatrixInverse(&D3DXMATRIX(), nullptr, &(GetCamera()->GetViewMatrix()));
		//逆行列から座標のみ取得.
		const D3DXVECTOR3 CameraPosition{ InverseViewMat._41,InverseViewMat._42,InverseViewMat._43 };

		D3DXVECTOR3 rayPos = CameraPosition;
		D3DXVECTOR3 rayDir{ 0.0f,0.0f,1.0f };
		D3DXVec3TransformNormal(&rayDir, &rayDir, &InverseViewMat);
		D3DXVec3Normalize(&rayDir, &rayDir);

		LPD3DXMESH mesh = nullptr;
		D3DXMATRIX matrix;
		D3DXMatrixIdentity(&matrix);

		float distance = 0.0f;

		std::list<RayInfo> infoList;
		RayInfo info;

		for (auto&& it : pGameObjectList) {

			if (it->GetType() == eWall) { continue; }

			if (!it->GetModel()) { continue; }

			mesh = it->GetModel()->GetMesh();
			matrix = it->GetMatrixConst();

			//レイが当たっていればlistに各情報を格納する.
			if (Intersect(mesh, matrix, rayPos, rayDir, &distance)) {
				info.mpGameObject = it.get();
				info.mDistance = distance;
				info.mRayDir = rayDir;
				info.mRayPos = rayPos;

				//あたっているメッシュのローカル座標.
				const D3DXVECTOR3 HitMeshPosition = rayPos + (rayDir * distance);

				info.mPosition = HitMeshPosition;

				infoList.push_back(info);
			}
		}

		//mDistanceをキーに昇順でソートする.
		infoList.sort([](const RayInfo& left, const RayInfo& right) { return left.mDistance < right.mDistance; });

		return infoList;
	}

	RayInfo Scene::RayTest(const D3DXVECTOR3& aPosition, const D3DXVECTOR3& aRayDirection, int aJudgeType)
	{
		SPtr<GameObject> pObject = FindGameObject(aJudgeType);
		if (!pObject) { return RayInfo{}; }

		float distance;

		LPD3DXMESH mesh = nullptr;
		D3DXMATRIX matrix;
		D3DXMatrixIdentity(&matrix);

		mesh = pObject->GetModel()->GetMesh();
		matrix = pObject->GetMatrixConst();

		RayInfo info;
		if (Intersect(mesh, matrix, aPosition, aRayDirection, &distance)) {
			info.mpGameObject = pObject.get();
			info.mDistance = distance;
		}
		return info;
	}
	float Scene::GetDeltaTime(void) const
	{
		return mpGameFrame->GetDeltaTime();
	}
}