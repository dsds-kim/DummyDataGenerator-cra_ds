# Implementation Plan

## Overview

PoC 구현을 3단계로 진행. 각 단계는 독립적으로 커밋하고 다음 단계로 넘어간다.

---

## Step 1: DB Connection Tests

**목표**: 기존 `DbConnection` 클래스의 동작을 검증하는 테스트 추가.

외부 테스트 프레임워크 없이 단순 assert 기반 테스트 실행기를 직접 구현 (PoC 범위).

### 구현 항목

- `Test/TestRunner.h` — 테스트 등록·실행·결과 출력 유틸리티
- `Test/DbConnectionTest.cpp` — `DbConnection` 테스트 케이스

### 테스트 케이스

| 테스트 | 검증 내용 |
|---|---|
| `connect_success` | 유효한 연결 문자열로 `connect()` 후 `isConnected() == true` |
| `disconnect_resets_state` | `disconnect()` 후 `isConnected() == false` |
| `double_connect_is_idempotent` | `connect()` 2회 호출해도 예외 없이 동일 상태 유지 |
| `connect_invalid_string_throws` | 잘못된 연결 문자열에서 `std::runtime_error` throw |
| `execute_without_connect_throws` | 연결 전 `executeNonQuery()` 호출 시 `std::runtime_error` throw |
| `execute_nonquery_insert` | 실제 INSERT 실행 후 DB에 행이 존재하는지 확인 |
| `execute_invalid_sql_throws` | 잘못된 SQL 실행 시 `std::runtime_error` throw |

### 완료 조건

- `main.cpp`에서 테스트 러너를 호출하면 모든 케이스 PASS/FAIL 출력
- 실제 DB 없이도 연결 실패 경로 케이스는 실행 가능해야 함

---

## Step 2: Dummy Data Generation — Test Code

**목표**: 더미 데이터 생성 기능 구현 전에 기대 동작을 테스트 코드로 먼저 정의 (TDD 방식).

### 생성할 인터페이스 (헤더만 작성, 구현은 Step 3)

```cpp
// DataGenerator.h
class DataGenerator {
public:
    std::wstring generateString(int maxLength);
    int          generateInt(int min, int max);
    double       generateDouble(int min, int max);
    std::wstring generateDate(const std::wstring& startDate, const std::wstring& endDate);
    std::wstring generateEmail();
    std::wstring buildInsertSql(const std::wstring& table,
                                const std::vector<ColumnDef>& columns);
};
```

### 테스트 케이스

| 테스트 | 검증 내용 |
|---|---|
| `string_length_within_max` | 생성된 문자열 길이 ≤ `maxLength` |
| `int_within_range` | 생성된 정수가 `[min, max]` 범위 내 |
| `double_within_range` | 생성된 실수가 `[min, max]` 범위 내 |
| `date_format_valid` | 생성된 날짜가 `YYYY-MM-DD` 형식 |
| `date_within_range` | 생성된 날짜가 `[startDate, endDate]` 범위 내 |
| `email_format_valid` | `@`와 도메인을 포함한 형식 |
| `build_insert_sql_shape` | 생성된 SQL이 `INSERT INTO table (cols) VALUES (...)` 형태 |

### 완료 조건

- 테스트를 실행하면 Step 3 미구현으로 인해 전부 FAIL (또는 링크 에러)
- 인터페이스(`DataGenerator.h`, `ColumnDef` 구조체)는 확정

---

## Step 3: Dummy Data Generation — Implementation

**목표**: Step 2의 테스트를 모두 통과하는 `DataGenerator` 구현. 생성된 데이터를 `DbConnection`으로 DB에 삽입.

### 구현 항목

- `DataGenerator.h` / `DataGenerator.cpp` — 타입별 랜덤값 생성 및 INSERT SQL 조립
- `SchemaConfig` — 대상 테이블·컬럼 정의를 코드에서 구성 (PoC이므로 파일 파싱 없이 하드코딩)
- `main.cpp` 업데이트 — 스키마 정의 → 데이터 생성 → DB 삽입 흐름 연결

### 컬럼 타입 지원 범위

| 타입 | 생성 방식 |
|---|---|
| `STRING` | 길이 제한 내 랜덤 알파벳+숫자 |
| `INT` | `[min, max]` 균등 분포 |
| `DOUBLE` | `[min, max]` 균등 분포 |
| `DATE` | `[startDate, endDate]` 범위 내 랜덤 날짜 |
| `EMAIL` | `<random>@<domain>` 형식 |

### 완료 조건

- Step 2의 모든 테스트 PASS
- `main.cpp` 실행 시 지정 건수만큼 DB에 행이 삽입됨

---

## 진행 현황

| 단계 | 상태 |
|---|---|
| Step 1: DB Connection Tests | 완료 |
| Step 2: Dummy Data Generation Tests | 미시작 |
| Step 3: Dummy Data Generation 구현 | 미시작 |
