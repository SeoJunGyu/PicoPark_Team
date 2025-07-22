# Level-Format Draft (v0.1) – 2025-07-22

## 1. Header
| key | type | desc |
| --- | --- | --- |
| version | int | 버전 |
| tileSize | int | 1 타일 사이즈 |
| gridWidth / gridHeight | int | 레벨 사이즈 |
| author | string | 맵 개발자 |
| created | string (ISO-8601) | 생성일 |
| description | string | 설명 |

### 1.1 Header JSON Example
```jsonc
{
  "version": 1,                    // schema version
  "tileSize": 16,                  // tile edge in pixels
  "gridWidth": 400,                // tiles
  "gridHeight": 225,               // tiles
  "author": "길하영",
  "created": "2025-07-22T11:00+09:00",
  "description": "더미 제작"
}
```
## 2. Layers[]
### 2.1 Common keys
| key | type | desc |
| --- | --- | --- |
| name | string | 레이어 식별용 이름 |
| type | enum{"TILE", "OBJECT"} | TILE → 타일 배열, OBJECT → 사각형·폴리곤 등 충돌 오브젝트 |
| visible | bool | 에디터/런타임에서 렌더링할지 여부 |
| parallax | float | 카메라 대비 스크롤 비율. 1.0=고정, 0.5=우측 느리게, 등 |
| opacity | float (0-1) (ISO-8601) | 투명도 |
| data | int[] or object[] | TILE → 타일 ID 배열 (1D 플랫)
OBJECT → 각 사각형 {x,y,w,h} 또는 임의 구조 |

### 2.2 TILE layer example
```json
{ "name":"BG", "type":"TILE", "parallax":0.5, "visible":true,
  "data":[ 4,4,4, … ] }
```
## 3. Entities[]
### 3.1 Common keys
| key | type | desc |
| --- | --- | --- |
| id | int | 고유 ID |
| type | string | "Key", "Door", "MovingPlatform" … |
| x, y | int | 타일좌표 또는 픽셀좌표 |
| properties | object | 기믹마다 다른 추가 필드 보관 |
| rotation | float | 도어·플랫폼 각도 등 |
| scale | 	float/vec2 | 	크기 배율 |

### 3.2 Entities layer example
```jsonc
"entities": [ { "id": 101, "type": "Key", "x": 64, "y": 144 },  { "id": 102, "type": "Door", "x": 368, "y": 96, "properties": { "locked": true } }, { "id": 201, "type": "MovingPlatform",  "x": 128, "y": 192,  "properties": { "path":[[128,192],[256,192]], "speed":80 } } ] 
```