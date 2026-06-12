# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## 프로젝트 개요

테스트용 더미 데이터를 생성하여 연결된 DB에 삽입하는 PoC 도구. Windows 콘솔 애플리케이션으로, C++20 기반.

## 빌드

**IDE**: Visual Studio (솔루션 파일: `DummyDataGenerator.slnx`)
**툴셋**: v145, C++20 (`stdcpp20`), Unicode 문자셋
**대상 플랫폼**: Windows 10 이상 (x86/x64)
**구성**: Debug|x64 (개발), Release|x64 (배포)

MSBuild CLI 빌드:
```
msbuild DummyDataGenerator.slnx /p:Configuration=Debug /p:Platform=x64
msbuild DummyDataGenerator.slnx /p:Configuration=Release /p:Platform=x64
```

출력 바이너리는 `x64/Debug/` 또는 `x64/Release/` 경로에 생성됨.

## 아키텍처 의도

PoC 단계이므로 아직 소스 코드가 없음. 구현 시 고려할 구조:

- **진입점**: `main()` — 설정 로드 → DB 연결 → 더미 데이터 생성 → DB 삽입 순서
- **DB 연결**: ODBC, OLE DB, 또는 외부 라이브러리(libpq, MySQL Connector 등) 선택 필요
- **데이터 생성**: 테이블 스키마 정의에 따라 각 컬럼 타입별 랜덤값 생성
- **설정**: DB 연결 문자열, 대상 테이블, 생성 건수 등을 외부 파일(JSON/INI)로 분리 권장

## 프로젝트 파일 구조

```
DummyDataGenerator/
├── DummyDataGenerator.slnx               # 솔루션 파일
└── DummyDataGenerator/
    ├── DummyDataGenerator.vcxproj        # 프로젝트 파일 (소스 파일 목록 관리)
    ├── DummyDataGenerator.vcxproj.filters # Visual Studio 파일 분류
    └── DummyDataGenerator.vcxproj.user   # 사용자별 로컬 설정 (git 제외 권장)
```

새 `.cpp`/`.h` 파일 추가 시 `.vcxproj`의 `<ItemGroup>`에 `<ClCompile>`/`<ClInclude>` 항목을 등록해야 빌드에 포함됨.
