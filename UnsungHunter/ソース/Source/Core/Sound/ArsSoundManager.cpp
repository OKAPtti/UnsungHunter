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

		//��Sound��T��
		for (; it != mSoundMap[aSoundName].end(); it++) {
			if ((*it).use_count() == 1 &&		//�}�l�[�W���ȊO�͎Q�Ƃ��Ă��Ȃ�
				!(*it)->IsPlaying()) {  //�Đ�����Ă��Ȃ�
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
			_ASSERT_EXPR(false, L"Sound�̖��O���Ⴂ�܂�");
		}
		return *it->second;
	}

	bool SoundManager::CreateSound(const std::string & aSoundName)
	{
		ComPtr<IDirectSoundBuffer8> pBuffer = nullptr;

		//�v���C�}���o�b�t�@��������ΐ���
		if (mBufferMap.find(aSoundName) == mBufferMap.end()) {
			pBuffer = CreateSecondaryBuffer(aSoundName);
			if (!pBuffer) { return false; }
			mBufferMap[aSoundName].push_back(pBuffer);

			//�ŏ��ɍ�����I���W�i���̓N���[���ɂ̂ݎg�����߁A�N���[�������
			pBuffer = CloneSecondaryBuffer(aSoundName, pBuffer.Get());
		}
		else {
			//�N���[�������
			pBuffer = CloneSecondaryBuffer(aSoundName,mBufferMap[aSoundName].back().Get());
		}

		mBufferMap[aSoundName].push_front(pBuffer);

		mSoundMap[aSoundName].push_front(std::make_shared<Sound>(pBuffer.Get()));

		return true;
	}

	ComPtr<IDirectSoundBuffer8> SoundManager::CreateSecondaryBuffer(const std::string & aSoundName)const
	{
		if (!mpDirectSound) {
			_ASSERT_EXPR(false, L"Initialze���Ă�ł�������");
			return nullptr;
		}

		//wav�̏��
		WAVEFORMATEX waveFormatEx;
		//�o�b�t�@�̃T�C�Y
		DWORD bufferByte = 0;
		//wave�̃f�[�^
		char* pWaveData = 0;
		//���f�[�^�̏ꏊ
		std::string soundPath = FixedSoundPath + aSoundName + ".wav";

		//wav�̏����擾
		ReadWave(soundPath.c_str(), &waveFormatEx, &bufferByte, &pWaveData);

		//DSBUFFERDESC�̐ݒ�
		DSBUFFERDESC DSBufferDesc;
		DSBufferDesc.dwSize = sizeof(DSBUFFERDESC);
		DSBufferDesc.dwFlags = DSBCAPS_GLOBALFOCUS | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY;
		DSBufferDesc.guid3DAlgorithm = DS3DALG_NO_VIRTUALIZATION;
		DSBufferDesc.dwBufferBytes = bufferByte;
		DSBufferDesc.dwReserved = 0;
		DSBufferDesc.lpwfxFormat = &waveFormatEx;
		DSBufferDesc.guid3DAlgorithm = GUID_NULL;

		//�Z�J���_���o�b�t�@�̍쐬(����ۑ����Ă����ꏊ)
		ComPtr<IDirectSoundBuffer8> outBuf = nullptr;
		ComPtr<IDirectSoundBuffer> pTmpBuf = nullptr;
		mpDirectSound->GetSoundDevice()->CreateSoundBuffer(&DSBufferDesc, &pTmpBuf, NULL);
		pTmpBuf->QueryInterface(IID_IDirectSoundBuffer8, (LPVOID*)&outBuf);

		// �Z�J���_���o�b�t�@��Wave�f�[�^����������
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

		//���f�[�^�̃R�s�[
		mpDirectSound->GetSoundDevice()->DuplicateSoundBuffer(aOriginalBuffer, &pTmpBuf);
		pTmpBuf->QueryInterface(IID_IDirectSoundBuffer8, (LPVOID*)&outBuf);

		return outBuf;
	}

	void SoundManager::ReadWave(const std::string & aPath, WAVEFORMATEX * apWaveFormatEx, DWORD * apBufferByte, char ** apWaveData)const
	{
		//���f�[�^����K�v�ȏ��𒊏o
		HMMIO hMmio = NULL;
		MMIOINFO mmioInfo;

		//wave�t�@�C���̃I�[�v��
		memset(&mmioInfo, 0, sizeof(MMIOINFO));
		hMmio = mmioOpen((LPSTR)aPath.c_str(), &mmioInfo, MMIO_READ);
		if (!hMmio) {
			_ASSERT_EXPR(false, L"�T�E���h�̖��O���Ⴄ���A�����t�@�C�������Ă��܂�");
		}

		// RIFF�`�����N������
		MMRESULT mmResult;
		MMCKINFO riffChunk;
		riffChunk.fccType = mmioFOURCC('W', 'A', 'V', 'E');
		mmResult = mmioDescend(hMmio, &riffChunk, NULL, MMIO_FINDRIFF);
		if (mmResult != MMSYSERR_NOERROR) {
			mmioClose(hMmio, 0);
			_ASSERT_EXPR(false, L"�ڂ��������ł��Ă��Ȃ��A�v�׋�");
		}

		// �t�H�[�}�b�g�`�����N������
		MMCKINFO formatChunk;
		formatChunk.ckid = mmioFOURCC('f', 'm', 't', ' ');
		mmResult = mmioDescend(hMmio, &formatChunk, &riffChunk, MMIO_FINDCHUNK);
		if (mmResult != MMSYSERR_NOERROR) {
			mmioClose(hMmio, 0);
			_ASSERT_EXPR(false, L"�ڂ��������ł��Ă��Ȃ��A�v�׋�");
		}

		//wave�̂Ȃ񂿂��\���̂Ƀf�[�^������
		DWORD fmsize = formatChunk.cksize;
		DWORD size = mmioRead(hMmio, (HPSTR)apWaveFormatEx, fmsize);
		if (size != fmsize) {
			mmioClose(hMmio, 0);
			_ASSERT_EXPR(false, L"�ڂ��������ł��Ă��Ȃ��A�v�׋�");
		}

		//�e�`�����N�ɖ߂�H
		mmioAscend(hMmio, &formatChunk, 0);

		// �f�[�^�`�����N����
		MMCKINFO dataChunk;
		dataChunk.ckid = mmioFOURCC('d', 'a', 't', 'a');
		mmResult = mmioDescend(hMmio, &dataChunk, &riffChunk, MMIO_FINDCHUNK);
		if (mmResult != MMSYSERR_NOERROR) {
			mmioClose(hMmio, 0);
			_ASSERT_EXPR(false, L"�ڂ��������ł��Ă��Ȃ��A�v�׋�");
		}

		*apWaveData = new char[dataChunk.cksize];
		size = mmioRead(hMmio, (HPSTR)*apWaveData, dataChunk.cksize);
		if (size != dataChunk.cksize) {
			delete[] * apWaveData;
			_ASSERT_EXPR(false, L"�ڂ��������ł��Ă��Ȃ��A�v�׋�");
		}
		*apBufferByte = size;

		mmioClose(hMmio, 0);
	}
}