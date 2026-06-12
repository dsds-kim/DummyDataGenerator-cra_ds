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

## 아키텍처

### DB 연결 (`DbConnection`)

Windows 내장 ODBC(`odbc32.lib`)를 사용하여 외부 라이브러리 없이 동작. SQL Server, MySQL, PostgreSQL 등 ODBC 드라이버가 설치된 모든 DB를 지원.

- `DbConnection(connectionString)` — 연결 문자열로 객체 생성
- `connect()` / `disconnect()` — ODBC 환경/연결 핸들 생명주기 관리
- `executeNonQuery(sql)` — INSERT, UPDATE, DELETE 실행; 실패 시 `std::runtime_error` throw

연결 문자열 예시:
```
// SQL Server
L"DRIVER={ODBC Driver 18 for SQL Server};SERVER=localhost;DATABASE=TestDB;UID=sa;PWD=password;"
// MySQL
L"DRIVER={MySQL ODBC 8.0 Driver};SERVER=localhost;DATABASE=TestDB;UID=root;PWD=password;"
// PostgreSQL
L"DRIVER={PostgreSQL Unicode};SERVER=localhost;DATABASE=TestDB;UID=postgres;PWD=password;"
```

### 흐름 (`main.cpp`)

`DbConnection` 생성 → `connect()` → `executeNonQuery()` 반복 → `disconnect()`

### 구현 현황

| 기능 | 상태 |
|---|---|
| DB 연결 (`DbConnection`) | 완료 |
| 더미 데이터 생성 | 미구현 |
| 생성된 데이터 DB 삽입 | 미구현 |

더미 데이터 생성 구현 시 고려할 사항:
- 대상 테이블 스키마 정의 방식 (코드 하드코딩 vs 외부 파일)
- 컬럼 타입별 랜덤값 생성 (문자열, 숫자, 날짜 등)
- 생성 건수 및 DB 연결 정보를 외부 설정 파일(JSON/INI)로 분리

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
