# TetrisMaster
건국대학교 객체지향 프로그래밍 수업에서 진행하는 프로젝트입니다.
제작자 : 김민재 정기헌 장우빈

## 절대 좌표 업데이트 

`Object::moveCursor(int x, int y)` / `Object::changeColor(int color)`

기존에 여러곳에서 쓰이던 전역 함수 gotoxy와 색상 변경 코드를 Object 클래스 내부로 통합.

`BlockMapManager()`, `Map()`, `Block()`, `GameState()`

모든 생성자가 객체 생성 시 화면 출력 기준점인 `(int abx, int aby)`를 매개변수로 받도록 함.

`BlockMapManager::showNextBlock()`

하드코딩된 X좌표 33을 제거하고, 메인 게임판 기준점(abx)으로부터 상대적인 거리(abx + 30)를 계산하여 그리도록 수정

`BlockMapManager::startBlock()`

NEXT 박스나 HOLD 박스에 있던 블록이 메인 보드로 나올 때, 좌표를 원상 복구하도록 함

`setOffset(int x, int y)`

객체가 그려질 화면상의 절대적인 기준점(도화지의 위치)을 변경하는 함수.

`Block::setCord(int x, int y)` : 상대좌표(내부 위치) 설정

설정된 기준점 안에서 객체가 위치할 상대 좌표를 지정

## HOLD

`holdShape`: 현재 보관된 블록의 모양을 기억하는 변수 (비어있을 때는 -1로 초기화)

`remainHolds`: 홀드 잔여 횟수를 저장하는 변수 (초기값 2, 레벨업 시 1씩 증가)

`holdCurrentBlock()` : 홀드 실행 (C입력시 호출), 현재 조작 중인 블록을 보관함에 넣고 다음 블록을 즉시 호출

`addHolds(int count)` : 홀드 횟수 충전(스테이지 클리어시 보상으로 횟수 충전하기 위해)

`showHoldBox()` : 홀드 전용 테두리 박스와 잔여 횟수 텍스트, 보관된 블록 표현 함수

# 일시정지, 종료

함수 추가 `x`, 메인 게임 루프인 `PLAY()` 내부에서 `SWITCH`문을 통해 구현


# Flip
## Map
`void flip()` : 현재 `totalBlock`을 뒤집음
`void stepGravity()` : `totalBlock`에 쌓여있는 블록들을 한칸씩 밑으로 이동
## BlockMapManager
`void flipMap()` : `Map`의 `flip()`을 호출한 뒤 천장에 붙어있는 블록들이 바닥에 붙을 때까지 `Map`의 `stepGravity()`를 호출
## TetrisGame
`int nextFlipTick`, `void scheduleNextFlip(int cur)` : 다음에 Flip을 할 Tick은 `scheduleNextFlip` 함수를 호출하여 `nextFlipTick`에 저장
# Random Line 생성
## Map
`void insertLine(int pos, int line[])` : `int line[]`으로 삽입할 줄을 입력받고 `pos`번째 줄에 삽입
## BlockMapManager
`void addRandomLine()` : 랜덤으로 12개의 정수(0 또는 1)을 생성하고 `Map`의 `insertLine`을 호출하여 가장 아래에 삽입
## TetrisGame
`int nextRandomTick`, `void scheduleNextRandom(int cur)` : 다음에 랜덤 줄을 생성할 Tick은 `scheduleNextRandom` 함수를 호출하여 `nextRandomTick`에 저장

# 일시정지 (Pause Menu)
BlockMapManager
void drawPauseMenu() : 화면 중앙에 "휴식 시간" 팝업창 UI를 출력
void erasePauseMenu() : 일시정지 해제 시 팝업창이 있던 영역을 공백으로 덮어써 잔상을 완벽하게 제거

TetrisGame
int handlePauseMenu(BlockMapManager& p1Manager, BlockMapManager* p2Manager = nullptr) : 일시정지 키(ESC, P)가 눌렸을 때 호출. while 루프를 돌며 입력을 대기하고, R을 누르면 맵을 복구하며 진행을 재개함. Q를 누르면 1을 반환하여 게임(메인 루프)을 종료. (멀티플레이 시 p2Manager를 전달하여 2P 화면 동시 복구 가능)

# 스테이지 클리어 (Stage Clear)
BlockMapManager
void drawStageClear() : 화면 중앙에 "일일 할당량 달성!" 팝업창 UI를 출력

TetrisGame
void handleStageClear() : 현재 클리어한 줄(lines)이 스테이지 요구량(clearLine)을 달성했는지 체크. 달성 시 클리어 팝업을 띄우고 레벨(level)을 증가시킨 뒤, 맵 초기화 및 HOLD 잔여 횟수를 1 추가함

# 콤보 이펙트 (Combo Effect)
BlockMapManager
void drawCombo(int count) : count가 2 이상일 때, 팝업 옆 랜덤한 짝수 좌표(글씨 깨짐 방지)에 ★ N 연속 처리 ★ 텍스트를 출력. 호출될 때마다 이전 좌표(lastComboX, lastComboY)에 남아있는 텍스트 잔상을 자동으로 지움

# 쓰레기봉투 애니메이션 (Trash Bag Animation)
BlockMapManager
int trashAnimTick : 애니메이션 동작 시간을 제어하는 타이머 변수. 줄을 지웠을 때(checkFullLine 호출 시) 30(1초)으로 세팅됨
void drawTrashBag() : 메인 루프 마지막에서 매 프레임 호출됨. trashAnimTick이 0보다 크면 값을 1씩 감소시키며 애니메이션 프레임(움직이는 봉투)을 교차 출력하고, 0 이하일 경우 기본 상태(정지된 봉투)를 출력
