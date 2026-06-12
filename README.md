# DummyDataGenerator

테스트용 더미 데이터를 생성하여 연결된 DB에 삽입하는 PoC 도구.

## 요구 사항

- Visual Studio 2022 (toolset v145)
- Windows 10 이상 (x64)
- ODBC 드라이버 (사용하는 DB에 맞는 드라이버 설치 필요)

| DB | ODBC 드라이버 |
|---|---|
| SQL Server | ODBC Driver 18 for SQL Server |
| MySQL | MySQL ODBC 8.0 Driver |
| PostgreSQL | PostgreSQL Unicode |

## 프로젝트 구조

솔루션에 두 개의 독립 실행 프로젝트가 포함되어 있습니다.

```
DummyDataGenerator.slnx
├── DummyDataGenerator/          # 앱 — 더미 데이터 생성 후 DB 삽입
└── DummyDataGeneratorTest/      # 테스트 — 실제 DB 없이 실행 가능
```

## 빌드

**Visual Studio IDE**

`DummyDataGenerator.slnx`를 열고 빌드 (`Ctrl+Shift+B`).

**MSBuild CLI**

```
msbuild DummyDataGenerator.slnx /p:Configuration=Debug /p:Platform=x64
msbuild DummyDataGenerator.slnx /p:Configuration=Release /p:Platform=x64
```

출력 바이너리: `x64/Debug/` 또는 `x64/Release/`

## 실행 방법

### 1. DB 및 테이블 준비

대상 DB에 데이터를 받을 테이블을 생성합니다. 예시 (SQL Server):

```sql
CREATE TABLE Users (
    Name      NVARCHAR(50),
    Age       INT,
    Score     FLOAT,
    BirthDate DATE,
    Email     NVARCHAR(100)
);
```

### 2. 연결 문자열 수정

`DummyDataGenerator/main.cpp` 상단의 `connStr`을 환경에 맞게 수정합니다.

```cpp
const std::wstring connStr =
    L"DRIVER={ODBC Driver 18 for SQL Server};"
    L"SERVER=localhost;"
    L"DATABASE=TestDB;"
    L"UID=sa;"
    L"PWD=password;";
```

### 3. 스키마 및 생성 건수 설정

`main.cpp`의 `rowCount`와 `schema`를 수정합니다.

```cpp
const int rowCount = 10;  // 삽입할 행 수

const std::vector<ColumnDef> schema = {
    { .name = L"Name",      .type = ColumnDef::Type::STRING, .maxLength = 50             },
    { .name = L"Age",       .type = ColumnDef::Type::INT,    .intMin = 18, .intMax = 80  },
    { .name = L"Score",     .type = ColumnDef::Type::DOUBLE, .dblMin = 0.0, .dblMax = 100.0 },
    { .name = L"BirthDate", .type = ColumnDef::Type::DATE,   .dateStart = L"1944-01-01", .dateEnd = L"2006-12-31" },
    { .name = L"Email",     .type = ColumnDef::Type::EMAIL                               },
};
```

**지원 컬럼 타입**

| 타입 | 필드 | 설명 |
|---|---|---|
| `STRING` | `maxLength` | 알파벳·숫자 랜덤 문자열 |
| `INT` | `intMin`, `intMax` | 범위 내 균등 분포 정수 |
| `DOUBLE` | `dblMin`, `dblMax` | 범위 내 균등 분포 실수 |
| `DATE` | `dateStart`, `dateEnd` | `YYYY-MM-DD` 형식 랜덤 날짜 |
| `EMAIL` | — | `<random>@<domain>` 형식 |

### 4. 실행

```
x64\Debug\DummyDataGenerator.exe
```

성공 시 출력:
```
DB connection successful
10 rows inserted into Users
```

## 테스트 방법

테스트 프로젝트(`DummyDataGeneratorTest`)는 **실제 DB 없이** 실행할 수 있습니다. Mock 기반으로 동작합니다.

### 실행

```
x64\Debug\DummyDataGeneratorTest.exe
```

### 출력 예시

```
=== DB Connection Tests ===

  [PASS] connect_invalid_string_throws
  [PASS] execute_without_connect_throws
  [PASS] connect_success
  [PASS] disconnect_resets_state
  [PASS] double_connect_is_idempotent
  [PASS] execute_nonquery_insert
  [PASS] execute_invalid_sql_throws

7 passed, 0 failed

=== Data Generator Tests ===

  [PASS] string_length_within_max
  [PASS] int_within_range
  [PASS] double_within_range
  [PASS] date_format_valid
  [PASS] date_within_range
  [PASS] email_format_valid
  [PASS] build_insert_sql_shape

7 passed, 0 failed
```

테스트가 실패하면 exit code `1`을 반환합니다.
