
#include <string>
#include <vector>

// ===================================================================
//  전역 상수 (정의는 tetris_master.cpp 에 있음)
// ===================================================================
extern const int SCREEN_WIDTH;   // 화면 너비
extern const int TRUCK_WIDTH;    // 청소차 그림 폭

// (ANSI 색상/제어 시퀀스 상수와 RAINBOW는 화면 그리기 내부 전용이므로
//  헤더에 노출하지 않고 tetris_master_impl.cpp 내부에 둔다.
//  이렇게 하면 다른 헤더의 동일 이름 상수(예: Color.h의 GREEN)와 충돌하지 않는다.)

// 시말서 데이터 파일 경로
extern const char* REPRIMAND_FILE;

// ===================================================================
//  자료형
// ===================================================================
// 블록 글꼴 한 글자 (5줄짜리)
struct Glyph {
    int w;
    const char* rows[5];
};

// 키 입력 종류
enum Key { K_UP, K_DOWN, K_ENTER, K_QUIT, K_BACK, K_OTHER };

// 시말서 한 건
struct Reprimand {
    std::string date;     // 접수일자
    std::string title;    // 사유 (목록에 표시)
    std::string detail;   // 경위서 본문 (여러 줄, '\n' 포함 가능)
};

// ===================================================================
//  화면 제어 / 출력 헬퍼
// ===================================================================
void clearScreen();   // 화면 전체 지우기 + 커서 좌상단
void cursorHome();    // 커서만 좌상단으로
void hardClearConsole(); // 콘솔 버퍼까지 확실히 비우기(게임 진입 전 잔상 제거)
void putLine(const std::string& content);  // 한 줄 출력(색 리셋 후 줄 지우기)
void putBlank();      // 빈 줄 출력(해당 줄 지움)
void endFrame();      // 한 화면 그리기 종료(아래 잔상 제거 + flush)
void initConsole();   // 윈도우 콘솔 초기화(ANSI 색상 + UTF-8 출력)

// ===================================================================
//  청소차 애니메이션
// ===================================================================
std::vector<std::string> makeTruck(char hub);          // 청소차 그림(바퀴 hub 문자)
std::string truckColorFor(int idx, int frame);          // 줄 인덱스별 색상
std::string drawShifted(int pos, const std::string& line); // pos만큼 민 줄
std::string buildRoad(int sweptUntil);                  // 도로 한 줄
void runTruckAnimation();                               // 청소차 1회 통과 애니메이션

// ===================================================================
//  타이틀 / 메뉴
// ===================================================================
Glyph glyph(char c);                                    // 글자 -> 블록 글꼴
int wordWidth(const std::string& word);                 // 블록 단어 폭
std::vector<std::string> renderWord(const std::string& word); // 블록 단어 5줄
int dispWidth(const std::string& s);                    // 화면 표시 폭(한글 2칸)
std::string pad(int n);                                 // 공백 n개
void drawTitleAndMenu(const std::vector<std::string>& items, int sel);

// ===================================================================
//  키 입력
// ===================================================================
Key readKey();

// ===================================================================
//  시말서 조회
// ===================================================================
std::string defaultReprimandFileContent();              // 기본 파일 내용
std::vector<Reprimand> loadReprimands();                // 파일에서 시말서 읽기
void drawReprimandList(const std::vector<Reprimand>& list, int sel);
void drawReprimandDetail(const Reprimand& r);
void writeReprimand();                                  // 시말서 작성 후 파일에 저장
void runReprimandScreen();                              // 시말서 조회 화면 전체 흐름

// ===================================================================
//  메인 메뉴 실행
// ===================================================================
// 콘솔 초기화 + 청소차 애니메이션 + 타이틀/메뉴 루프를 모두 실행한다.
// 사용자가 선택한 결과를 반환한다:
//   0 = SINGLE,  1 = 2P,  -1 = 종료(q)
// (시말서 조회는 함수 내부에서 처리되고 메뉴로 복귀하므로 반환되지 않는다)
int runTetrisMaster();