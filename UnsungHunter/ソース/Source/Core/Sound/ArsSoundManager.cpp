#include <mmsystem.h>

#pragma comment ( lib, "winmm.lib" )

#include"ArsSoundManager.h"
#include"../DirectX/ArsDirectSound.h"
#include"../ArsConfiguration.h"

namespace Alrescha {
	SoundManager::SoundManager(void)
		:mpDirectSound(nullptr)
		, mSoundMap()
		, mBufferMap()
		, mLoopSoundMap()
	{

	}

	SoundManager::~SoundManager(void)
	{

	}

	void SoundManager::Initialize(HWND apHwnd)
	{
		mpDirectSound.reset(new DirectSound(apHwnd));
	}

	void SoundManager::Finalize(void)
	{
		mBufferMap.clear();
		mSoundMap.clear();
	}

	void SoundManager::Update(void)
	{
		for (auto&& [key,val] : mLoopSoundMap) {
			if (!val->IsPlaying()) { val->Play(); }
		}
	}

	SPtr<Sound> SoundManager::GetSound(const std::string & aSoundName, UINT aVolume)
	{
		auto it = mSoundMap[aSoundName].begin();

		bool needCreate = true;

		//空きSoundを探す
		for (; it != mSoundMap[aSoundName].end(); it++) {
			if ((*it).use_count() == 1 &&		//マネージャ以外は参照していない
				!(*it)->IsPlaying()) {  //再生されていない
				needCreate = false;
				break;
			}
		}
	
		if (needCreate) {
			bool result = CreateSound(aSoundName);
			if (!result) { return nullptr; }

			it = mSoundMap[aSoundName].begin();
		}

		(*it)->SetVolume(aVolume);
		(*it)->SetSpeed(1.0f);

		return *it;
	}

	void SoundManager::Play(const std::string & aSoundName, UINT aVolume)
	{
		auto pSound = GetSound(aSoundName, aVolume);
		if (pSound) { pSound->Play(); }
	}

	void SoundManager::AddLoopSound(const std::string& aSoundName)
	{
		if (mLoopSoundMap.find(aSoundName) == mLoopSoundMap.end()) {
			auto pSound = GetSound(aSoundName);
			pSound->Play();
			mLoopSoundMap[aSoundName] = pSound;
		}
	}

	void SoundManager::RemoveLoopSound(const std::string& aSoundName)
	{
		if (mLoopSoundMap.find(aSoundName) != mLoopSoundMap.end()) {
			mLoopSoundMap.erase(aSoundName);
		}
	}

	Sound& SoundManager::GetLoopSound(const std::string& aSoundName)
	{
		auto it = mLoopSoundMap.find(aSoundName);
		if (it == mLoopSoundMap.end()) {
			_ASSERT_EXPR(false, L"Soundの名前が違います");
		}
		return *it->second;
	}

	bool SoundManager::CreateSound(const std::string & aSoundName)
	{
		ComPtr<IDirectSoundBuffer8> pBuffer = nullptr;

		//プライマリバッファが無ければ生成
		if (mBufferMap.find(aSoundName) == mBufferMap.end()) {
			pBuffer = CreateSecondaryBuffer(aSoundName);
			if (!pBuffer) { return false; }
			mBufferMap[aSoundName].push_back(pBuffer);

			//最初に作ったオリジナルはクローンにのみ使うため、クローンを作る
			pBuffer = CloneSecondaryBuffer(aSoundName, pBuffer.Get());
		}
		else {
			//クローンを作る
			pBuffer = CloneSecondaryBuffer(aSoundName,mBufferMap[aSoundName].back().Get());
		}

		mBufferMap[aSoundName].push_front(pBuffer);

		mSoundMap[aSoundName].push_front(std::make_shared<Sound>(pBuffer.Get()));

		return true;
	}

	ComPtr<IDirectSoundBuffer8> SoundManager::CreateSecondaryBuffer(const std::string & aSoundName)const
	{
		if (!mpDirectSound) {
			_ASSERT_EXPR(false, L"Initialzeを呼んでください");
			return nullptr;
		}

		//wavの情報
		WAVEFORMATEX waveFormatEx;
		//バッファのサイズ
		DWORD bufferByte = 0;
		//waveのデータ
		char* pWaveData = 0;
		//音データの場所
		std::string soundPath = FixedSoundPath + aSoundName + ".wav";

		//wavの情報を取得
		ReadWave(soundPath.c_str(), &waveFormatEx, &bufferByte, &pWaveData);

		//DSBUFFERDESCの設定
		DSBUFFERDESC DSBufferDesc;
		DSBufferDesc.dwSize = sizeof(DSBUFFERDESC);
		DSBufferDesc.dwFlags = DSBCAPS_GLOBALFOCUS | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY;
		DSBufferDesc.guid3DAlgorithm = DS3DALG_NO_VIRTUALIZATION;
		DSBufferDesc.dwBufferBytes = bufferByte;
		DSBufferDesc.dwReserved = 0;
		DSBufferDesc.lpwfxFormat = &waveFormatEx;
		DSBufferDesc.guid3DAlgorithm = GUID_NULL;

		//セカンダリバッファの作成(音を保存しておく場所)
		ComPtr<IDirectSoundBuffer8> outBuf = nullptr;
		ComPtr<IDirectSoundBuffer> pTmpBuf = nullptr;
		mpDirectSound->GetSoundDevice()->CreateSoundBuffer(&DSBufferDesc, &pTmpBuf, NULL);
		pTmpBuf->QueryInterface(IID_IDirectSoundBuffer8, (LPVOID*)&outBuf);

		// セカンダリバッファにWaveデータを書き込む
		LPVOID lpvWrite = 0;
		DWORD dwLength = 0;
		if (DS_OK == outBuf->Lock(0, 0, &lpvWrite, &dwLength, NULL, NULL, DSBLOCK_ENTIREBUFFER)) {
			memcpy(reinterpret_cast<LPVOID>(lpvWrite), reinterpret_cast<const void*>(pWaveData), dwLength);
			outBuf->Unlock(lpvWrite, dwLength, NULL, 0);
		}
		delete[] pWaveData;

		return outBuf;
	}

	ComPtr<IDirectSoundBuffer8> SoundManager::CloneSecondaryBuffer(const std::string & aSoundName,const LPDIRECTSOUNDBUFFER& aOriginalBuffer)const
	{
		ComPtr<IDirectSoundBuffer> pTmpBuf = nullptr;
		ComPtr<IDirectSoundBuffer8> outBuf;

		//音データのコピー
		mpDirectSound->GetSoundDevice()->DuplicateSoundBuffer(aOriginalBuffer, &pTmpBuf);
		pTmpBuf->QueryInterface(IID_IDirectSoundBuffer8, (LPVOID*)&outBuf);

		return outBuf;
	}

	void SoundManager::ReadWave(const std::string & aPath, WAVEFORMATEX * apWaveFormatEx, DWORD * apBufferByte, char ** apWaveData)const
	{
		//音データから必要な情報を抽出
		HMMIO hMmio = NULL;
		MMIOINFO mmioInfo;

		//waveファイルのオープン
		memset(&mmioInfo, 0, sizeof(MMIOINFO));
		hMmio = mmioOpen((LPSTR)aPath.c_str(), &mmioInfo, MMIO_READ);
		if (!hMmio) {
			_ASSERT_EXPR(false, L"サウンドの名前が違うか、音声ファイルが壊れています");
		}

		// RIFFチャンクを見る
		MMRESULT mmResult;
		MMCKINFO riffChunk;
		riffChunk.fccType = mmioFOURCC('W', 'A', 'V', 'E');
		mmResult = mmioDescend(hMmio, &riffChunk, NULL, MMIO_FINDRIFF);
		if (mmResult != MMSYSERR_NOERROR) {
			mmioClose(hMmio, 0);
			_ASSERT_EXPR(false, L"詳しく理解できていない、要勉強");
		}

		// フォーマットチャンクを見る
		MMCKINFO formatChunk;
		formatChunk.ckid = mmioFOURCC('f', 'm', 't', ' ');
		mmResult = mmioDescend(hMmio, &formatChunk, &riffChunk, MMIO_FINDCHUNK);
		if (mmResult != MMSYSERR_NOERROR) {
			mmioClose(hMmio, 0);
			_ASSERT_EXPR(false, L"詳しく理解できていない、要勉強");
		}

		//waveのなんちゃら構造体にデータを入れる
		DWORD fmsize = formatChunk.cksize;
		DWORD size = mmioRead(hMmio, (HPSTR)apWaveFormatEx, fmsize);
		if (size != fmsize) {
			mmioClose(hMmio, 0);
			_ASSERT_EXPR(false, L"詳しく理解できていない、要勉強");
		}

		//親チャンクに戻る？
		mmioAscend(hMmio, &formatChunk, 0);

		// データチャンク検索
		MMCKINFO dataChunk;
		dataChunk.ckid = mmioFOURCC('d', 'a', 't', 'a');
		mmResult = mmioDescend(hMmio, &dataChunk, &riffChunk, MMIO_FINDCHUNK);
		if (mmResult != MMSYSERR_NOERROR) {
			mmioClose(hMmio, 0);
			_ASSERT_EXPR(false, L"詳しく理解できていない、要勉強");
		}

		*apWaveData = new char[dataChunk.cksize];
		size = mmioRead(hMmio, (HPSTR)*apWaveData, dataChunk.cksize);
		if (size != dataChunk.cksize) {
			delete[] * apWaveData;
			_ASSERT_EXPR(false, L"詳しく理解できていない、要勉強");
		}
		*apBufferByte = size;

		mmioClose(hMmio, 0);
	}
}