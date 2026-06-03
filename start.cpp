// TETRIS MASTER - 구현부 (정의)
// 라이브러리 본체. 진입점(main)은 main.cpp 에 있음.
// 소스는 CP949(코드 페이지 949)로 저장하고 /utf-8 옵션 없이 컴파일하세요.
#include "start.h"

#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <fstream>
#include <sstream>
#include <windows.h>
#include <conio.h>
#include <limits>

using namespace std;

const int SCREEN_WIDTH = 80;
const int TRUCK_WIDTH = 30;

// ----- ANSI 색상 (이 파일 내부 전용: static 내부 링키지로 한정) -----
// static을 붙여 다른 번역 단위(특히 Color.h의 enum GREEN)와 심볼이 충돌하지 않게 한다.
static const string RESET = "\033[0m";
static const string BODY = "\033[1;93m";
static const string TEXT = "\033[1;96m";
static const string WHEEL = "\033[1;97m";
static const string BEACON1 = "\033[1;91m";
static const string BEACON2 = "\033[1;93m";
static const string GREEN = "\033[32m";
static const string DIM = "\033[90m";

// 타이틀 무지개색 (글자마다 순환)
static const vector<string> RAINBOW = {
    "\033[1;91m", "\033[1;93m", "\033[1;92m",
    "\033[1;96m", "\033[1;94m", "\033[1;95m",
};

void clearScreen() { cout << "\033[2J\033[H"; }  // 화면 전체 지우기
void cursorHome() { cout << "\033[H"; }          // 커서만 좌상단으로
static const string CLR_EOL = "\033[K";           // 현재 줄에서 커서 오른쪽 지우기
static const string CLR_DOWN = "\033[J";           // 커서 아래 화면 전체 지우기

// 한 줄 출력: 항상 색을 리셋한 뒤 줄 끝을 지우고 개행한다.
// (색 배경이 줄 끝까지 번지는 잔상을 막기 위해 RESET -> CLR_EOL 순서가 중요)
void putLine(const string& content) {
    cout << content << RESET << CLR_EOL << "\n";
}
// 빈 줄 출력 (해당 줄을 깨끗이 지움)
void putBlank() {
    cout << RESET << CLR_EOL << "\n";
}
// 한 화면 그리기를 끝낼 때: 커서 아래 남은 줄을 모두 비운다.
void endFrame() {
    cout << RESET << CLR_DOWN;
    cout.flush();
}

// 윈도우 콘솔 초기화: ANSI 색상 활성화(Win10+) + UTF-8 출력
void initConsole() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
    DWORD mode = 0;
    GetConsoleMode(hOut, &mode);
    SetConsoleMode(hOut, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    // 출력 코드페이지는 콘솔 기본(CP949)을 그대로 사용한다.
    // (기존 테트리스 게임과 인코딩을 맞추기 위해 UTF-8 설정을 쓰지 않음)
}

// 윈도우 콘솔 버퍼 전체를 공백으로 채우고 커서를 좌상단으로 보낸다.
// ANSI \033[2J 가 conhost에서 완전히 지우지 못하는 경우(잔상)를 막기 위해,
// 게임으로 넘어가기 직전 등 확실한 클리어가 필요할 때 사용한다.
void hardClearConsole() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (!GetConsoleScreenBufferInfo(hOut, &csbi)) return;
    DWORD cells = (DWORD)csbi.dwSize.X * (DWORD)csbi.dwSize.Y;
    COORD home = { 0, 0 };
    DWORD written;
    FillConsoleOutputCharacterA(hOut, ' ', cells, home, &written);            // 글자 비우기
    FillConsoleOutputAttribute(hOut, csbi.wAttributes, cells, home, &written); // 색 속성 초기화
    SetConsoleCursorPosition(hOut, home);                                      // 커서 좌상단
}

// ===================================================================
//  1) 청소차 애니메이션 (한 번 통과)
// ===================================================================
vector<string> makeTruck(char hub) {
    string h(1, hub);
    return {
        "             (*)            ",
        "        ___________________ ",
        "       |   _______________  |",
        "       |  |  C L E A N    | |",
        "    ___|  |_______________| |",
        "   |                        |",
        "   |________________________|",
        "   |    ____        ____     |",
        "   |   /    \\      /    \\    |",
        "   |__( " + h + h + " )____( " + h + h + " )___|",
        "       \\____/      \\____/    ",
    };
}

string truckColorFor(int idx, int frame) {
    if (idx == 0) return (frame % 2 == 0) ? BEACON1 : BEACON2;
    if (idx == 3) return TEXT;
    if (idx >= 7) return WHEEL;
    return BODY;
}

string drawShifted(int pos, const string& line) {
    int start = (pos < 0) ? -pos : 0;
    if (start >= (int)line.size()) return string(SCREEN_WIDTH, ' ');
    int leading = (pos > 0) ? pos : 0;
    if (leading >= SCREEN_WIDTH) return string(SCREEN_WIDTH, ' ');
    int avail = SCREEN_WIDTH - leading;
    string visible = line.substr(start, avail);
    string out = string(leading, ' ') + visible;
    if ((int)out.size() < SCREEN_WIDTH)
        out += string(SCREEN_WIDTH - out.size(), ' ');
    return out;
}

string buildRoad(int sweptUntil) {
    const string dust = ".,'*";
    string road = GREEN;
    bool inDust = false;
    for (int x = 0; x < SCREEN_WIDTH; ++x) {
        if (x < sweptUntil) {
            if (inDust) { road += GREEN; inDust = false; }
            road += '_';
        }
        else {
            if (!inDust) { road += DIM; inDust = true; }
            road += dust[x % dust.size()];
        }
    }
    road += RESET;
    return road;
}

void runTruckAnimation() {
    const char hubs[] = { '|', '/', '-', '\\' };
    clearScreen();                       // 시작 시 한 번만 전체 지우기
    int frame = 0;
    for (int pos = -TRUCK_WIDTH; pos <= SCREEN_WIDTH; ++pos, ++frame) {
        cursorHome();                    // 매 프레임 커서만 좌상단으로
        for (int i = 0; i < 5; ++i) putBlank();   // 위쪽 여백 5줄
        vector<string> truck = makeTruck(hubs[frame % 4]);
        for (int i = 0; i < (int)truck.size(); ++i)
            putLine(truckColorFor(i, frame) + drawShifted(pos, truck[i]));
        putLine(buildRoad(pos + TRUCK_WIDTH - 4));
        endFrame();                      // 아래쪽 잔상 제거 + flush
        this_thread::sleep_for(chrono::milliseconds(60));
    }
    this_thread::sleep_for(chrono::milliseconds(250));
}

// ===================================================================
//  2) TETRIS MASTER 타이틀 (블록 문자)
// ===================================================================
Glyph glyph(char c) {
    switch (c) {
    case 'T': return { 5, {"▒▒▒▒▒","  ▒  ","  ▒  ","  ▒  ","  ▒  "} };
    case 'E': return { 4, {"▒▒▒▒","▒   ","▒▒▒ ","▒   ","▒▒▒▒"} };
    case 'R': return { 4, {"▒▒▒ ","▒  ▒","▒▒▒ ","▒ ▒ ","▒  ▒"} };
    case 'I': return { 3, {"▒▒▒"," ▒ "," ▒ "," ▒ ","▒▒▒"} };
    case 'S': return { 4, {"▒▒▒▒","▒   ","▒▒▒ ","   ▒","▒▒▒▒"} };
    case 'M': return { 5, {"▒   ▒","▒▒ ▒▒","▒ ▒ ▒","▒   ▒","▒   ▒"} };
    case 'A': return { 5, {" ▒▒▒ ","▒   ▒","▒▒▒▒▒","▒   ▒","▒   ▒"} };
    default:  return { 3, {"   ","   ","   ","   ","   "} };
    }
}

int wordWidth(const string& word) {
    int w = 0;
    for (size_t i = 0; i < word.size(); ++i) {
        w += glyph(word[i]).w;
        if (i + 1 < word.size()) w += 1;   // 글자 사이 간격
    }
    return w;
}

vector<string> renderWord(const string& word) {
    vector<string> out(5, "");
    for (size_t i = 0; i < word.size(); ++i) {
        Glyph g = glyph(word[i]);
        string col = RAINBOW[i % RAINBOW.size()];
        for (int r = 0; r < 5; ++r) {
            out[r] += col;
            out[r] += g.rows[r];
            out[r] += RESET;
            out[r] += " ";
        }
    }
    return out;
}

// UTF-8 표시 폭 (한글 등 3바이트 문자는 2칸)
int dispWidth(const string& s) {
    int w = 0;
    for (size_t i = 0; i < s.size();) {
        unsigned char c = s[i];
        if (c < 0x80) { w += 1; i += 1; }
        else if ((c >> 5) == 0x6) { w += 2; i += 2; }
        else if ((c >> 4) == 0xE) { w += 2; i += 3; }
        else if ((c >> 3) == 0x1E) { w += 2; i += 4; }
        else { w += 1; i += 1; }
    }
    return w;
}

string pad(int n) { return n > 0 ? string(n, ' ') : ""; }

void drawTitleAndMenu(const vector<string>& items, int sel) {
    cursorHome();                         // 전체 지우기 대신 커서만 좌상단으로
    putBlank(); putBlank();

    int barLen = 40;
    putLine(pad((SCREEN_WIDTH - barLen) / 2) + "\033[1;95m" + string(barLen, '='));
    putBlank();

    vector<string> w1 = renderWord("TETRIS");
    int width1 = wordWidth("TETRIS");
    for (int r = 0; r < 5; ++r)
        putLine(pad((SCREEN_WIDTH - width1) / 2) + w1[r]);
    putBlank();

    vector<string> w2 = renderWord("MASTER");
    int width2 = wordWidth("MASTER");
    for (int r = 0; r < 5; ++r)
        putLine(pad((SCREEN_WIDTH - width2) / 2) + w2[r]);
    putBlank();

    string sub = "~  C + +   C L I   E D I T I O N  ~";
    putLine(pad((SCREEN_WIDTH - dispWidth(sub)) / 2) + DIM + sub);

    putLine(pad((SCREEN_WIDTH - barLen) / 2) + "\033[1;95m" + string(barLen, '='));
    putBlank(); putBlank();

    for (int i = 0; i < (int)items.size(); ++i) {
        string text, color;
        if (i == sel) {
            text = ">> " + items[i] + " <<";
            color = "\033[1;30;106m";   // 밝은 청록 배경 + 검은 글씨 (선택됨)
        }
        else {
            text = "   " + items[i] + "   ";
            color = "\033[90m";          // 회색 (비선택)
        }
        putLine(pad((SCREEN_WIDTH - dispWidth(text)) / 2) + color + text);
        putBlank();
    }

    putBlank();
    string hint = "[UP/DOWN] 이동    [Enter] 선택    [q] 종료";
    putLine(pad((SCREEN_WIDTH - dispWidth(hint)) / 2) + DIM + hint);
    endFrame();                            // 아래쪽 잔상 제거 + flush
}

// ===================================================================
//  3) 키 입력 (방향키) - _getch()
// ===================================================================
Key readKey() {
    int c = _getch();
    if (c == '\r')             return K_ENTER;
    if (c == 8)                return K_BACK;   // Backspace
    if (c == 27)               return K_BACK;   // ESC -> 뒤로가기
    if (c == 'q' || c == 'Q')  return K_QUIT;
    if (c == 0 || c == 224) {                    // 방향키 prefix
        int c2 = _getch();
        if (c2 == 72) return K_UP;
        if (c2 == 80) return K_DOWN;
    }
    return K_OTHER;
}

// ===================================================================
//  4) 시말서 조회 화면
// ===================================================================
// 시말서 데이터 파일 경로 (실행 파일과 같은 폴더)
const char* REPRIMAND_FILE = "reprimands.txt";

// 기본 시말서 내용 (파일이 없을 때 이 내용으로 파일을 새로 만든다)
string defaultReprimandFileContent() {
    return
        "# 시말서 데이터 파일\n"
        "# 형식: date: / title: 줄 뒤에 경위서 본문을 쓰고, --- 로 레코드를 구분합니다.\n"
        "# '#'으로 시작하는 줄은 주석입니다.\n"
        "\n"
        "date: 2026-05-21\n"
        "title: I-블록을 4줄 구멍에 안 꽂음\n"
        "본인은 2026년 5월 21일, 우측 끝에 완벽한 4줄 우물이\n"
        "준비되어 있었음에도 불구하고 마침 등장한 I-블록을\n"
        "엉뚱한 곳에 눕혀 테트리스 기회를 날렸습니다.\n"
        "이에 깊이 반성하며 다시는 I-블록을 낭비하지 않을 것을\n"
        "굳게 다짐합니다.\n"
        "---\n"
        "date: 2026-05-18\n"
        "title: T-스핀 자랑하다 탑아웃\n"
        "본인은 화려한 T-스핀 더블을 성공시킨 직후, 승리를\n"
        "확신하고 방심하여 다음 블록 배치를 소홀히 한 결과\n"
        "보드 천장에 블록을 쌓아 게임 오버(탑아웃)에\n"
        "이르렀습니다. 자만은 패배의 어머니임을 통감합니다.\n"
        "---\n"
        "date: 2026-05-15\n"
        "title: 하드드롭 오발로 자폭\n"
        "본인은 소프트드롭을 의도하였으나 스페이스바를\n"
        "오타하여 블록을 즉시 하드드롭시켰고, 그 결과\n"
        "쌓아두었던 라인이 어긋나 3콤보가 무산되었습니다.\n"
        "키 입력에 더욱 신중을 기하겠습니다.\n"
        "---\n"
        "date: 2026-05-10\n"
        "title: 홀드 기능 무단 남용\n"
        "본인은 한 게임에서 홀드(Hold) 기능을 과도하게\n"
        "사용하여 같은 블록을 17회 교체하였으며, 이로 인해\n"
        "게임 진행이 지연되고 동료 2P의 원성을 샀습니다.\n"
        "앞으로는 홀드를 계획적으로 사용하겠습니다.\n"
        "---\n"
        "date: 2026-05-03\n"
        "title: 라인 클리어 후 환호성 소음\n"
        "본인은 테트리스(4줄 동시 클리어) 성공 시 과도하게\n"
        "큰 함성을 질러 인근 좌석의 정숙을 해쳤습니다.\n"
        "기쁨은 마음속으로 간직하겠습니다.\n";
}

// 줄 끝의 \r 제거 (윈도우 CRLF 대응)
static void rstripCR(string& s) {
    if (!s.empty() && s.back() == '\r') s.pop_back();
}

// "key:" 접두사로 시작하면 값을 trim해서 반환, 아니면 false
static bool parseField(const string& line, const string& key, string& out) {
    if (line.rfind(key, 0) == 0) {            // line이 key로 시작
        string v = line.substr(key.size());
        size_t b = v.find_first_not_of(" \t");
        out = (b == string::npos) ? "" : v.substr(b);
        return true;
    }
    return false;
}

// 파일에서 시말서 목록을 읽어온다. 파일이 없으면 기본 파일을 생성한다.
vector<Reprimand> loadReprimands() {
    // 파일이 없으면 기본 내용으로 생성
    {
        ifstream check(REPRIMAND_FILE, ios::binary);
        if (!check.good()) {
            ofstream out(REPRIMAND_FILE, ios::binary);
            if (out.good()) out << defaultReprimandFileContent();
        }
    }

    vector<Reprimand> list;
    ifstream in(REPRIMAND_FILE, ios::binary);
    if (!in.good()) return list;              // 그래도 못 열면 빈 목록

    Reprimand cur;
    bool hasCur = false;
    string detailBuf;
    string line;

    auto flush = [&]() {
        if (hasCur) {
            // detailBuf 끝의 개행 제거
            while (!detailBuf.empty() &&
                (detailBuf.back() == '\n' || detailBuf.back() == '\r'))
                detailBuf.pop_back();
            cur.detail = detailBuf;
            list.push_back(cur);
        }
        cur = Reprimand();
        detailBuf.clear();
        hasCur = false;
        };

    while (getline(in, line)) {
        rstripCR(line);
        if (line.rfind("\xEF\xBB\xBF", 0) == 0) line = line.substr(3); // BOM 제거
        if (!line.empty() && line[0] == '#') continue;                  // 주석
        if (line == "---") { flush(); continue; }                       // 레코드 구분

        string v;
        if (parseField(line, "date:", v)) { cur.date = v;  hasCur = true; continue; }
        if (parseField(line, "title:", v)) { cur.title = v; hasCur = true; continue; }

        // 그 외 줄은 경위서 본문
        if (hasCur) {
            detailBuf += line;
            detailBuf += "\n";
        }
    }
    flush();   // 마지막 레코드 처리
    return list;
}

// 시말서 목록 화면
void drawReprimandList(const vector<Reprimand>& list, int sel) {
    cursorHome();
    putBlank(); putBlank();

    int barLen = 50;
    string head = "[ 시 말 서 조 회 ]";
    putLine(pad((SCREEN_WIDTH - barLen) / 2) + "\033[1;93m" + string(barLen, '='));
    putLine(pad((SCREEN_WIDTH - dispWidth(head)) / 2) + "\033[1;93m" + head);
    putLine(pad((SCREEN_WIDTH - barLen) / 2) + "\033[1;93m" + string(barLen, '='));
    putBlank();

    putLine(pad((SCREEN_WIDTH - barLen) / 2) + DIM + "총 " +
        to_string(list.size()) + "건의 시말서가 접수되었습니다.");
    putBlank();

    // 목록 항목
    for (int i = 0; i < (int)list.size(); ++i) {
        string line = list[i].date + "   " + list[i].title;
        string text, color;
        if (i == sel) {
            text = " > " + line + " ";
            color = "\033[1;30;103m";   // 노란 배경 + 검은 글씨 (선택됨)
        }
        else {
            text = "   " + line + " ";
            color = "\033[0;37m";        // 흰색 (비선택)
        }
        putLine(pad((SCREEN_WIDTH - barLen) / 2) + color + text);
        putBlank();
    }

    putBlank();
    string hint = "[UP/DOWN] 이동    [Enter] 상세보기    [ESC] 메뉴로";
    putLine(pad((SCREEN_WIDTH - dispWidth(hint)) / 2) + DIM + hint);
    endFrame();                            // 아래쪽 잔상 제거 + flush
}

// 시말서 상세 화면
void drawReprimandDetail(const Reprimand& r) {
    cursorHome();
    putBlank(); putBlank();

    int boxLeft = 14;
    int barLen = SCREEN_WIDTH - boxLeft * 2;

    putLine(pad(boxLeft) + "\033[1;97m" + string(barLen, '-'));
    putLine(pad(boxLeft) + "\033[1;97m" + "시 말 서");
    putLine(pad(boxLeft) + "\033[1;97m" + string(barLen, '-'));
    putBlank();

    putLine(pad(boxLeft) + "\033[1;96m" + "접수일자: " + RESET + r.date);
    putLine(pad(boxLeft) + "\033[1;96m" + "사    유: " + RESET + "\033[1;91m" + r.title);
    putBlank();

    putLine(pad(boxLeft) + "\033[1;96m" + "[ 경 위 서 ]");
    putBlank();

    // detail은 여러 줄(\n 포함) -> 줄마다 들여쓰기
    string line;
    for (size_t i = 0; i <= r.detail.size(); ++i) {
        if (i == r.detail.size() || r.detail[i] == '\n') {
            putLine(pad(boxLeft) + line);
            line.clear();
        }
        else {
            line += r.detail[i];
        }
    }

    putBlank();
    putLine(pad(boxLeft) + "\033[1;97m" + string(barLen, '-'));
    putBlank();

    string hint = "[ESC / Backspace] 목록으로 돌아가기";
    putLine(pad((SCREEN_WIDTH - dispWidth(hint)) / 2) + DIM + hint);
    endFrame();                            // 아래쪽 잔상 제거 + flush
}

// 시말서 조회 화면 전체 흐름 (목록 <-> 상세). ESC로 메뉴 복귀.
// 시말서 작성: 화면을 지우고 입력받아 파일에 추가한다.
// (입력·출력·파일 모두 콘솔 기본 코드페이지 CP949로 일관되므로 변환이 필요 없다)
void writeReprimand() {
    clearScreen();
    cout << "\033[?25h";          // 입력 동안 커서 보이게
    cout << RESET;

    FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));

    cout << "=== 시말서 작성 ===\n\n";

    string date, title, line, detail;

    cout << "접수일자 (예: 2026-05-31): ";
    getline(cin, date);

    cout << "사유(제목): ";
    getline(cin, title);

    cout << "경위서 본문 입력 (빈 줄에서 Enter 누르면 종료):\n";
    while (true) {
        getline(cin, line);
        if (line.empty()) break;          // 빈 줄이면 본문 입력 종료
        detail += line;
        detail += "\n";
    }

    // 기존 레코드와 구분하기 위해 --- 를 먼저 쓴 뒤 새 레코드를 추가한다.
    ofstream out(REPRIMAND_FILE, ios::app | ios::binary);
    if (out.good()) {
        out << "\n---\n";
        out << "date: " << date << "\n";
        out << "title: " << title << "\n";
        out << detail;
        cout << "\n저장되었습니다. [아무 키] 메뉴로 돌아가기";
    }
    else {
        cout << "\n파일을 열 수 없어 저장하지 못했습니다. [아무 키] 계속";
    }
    cout.flush();
    cout << "\033[?25l";          // 커서 다시 숨김
    readKey();
}

void runReprimandScreen() {
    vector<Reprimand> list = loadReprimands();
    int sel = 0;
    bool inScreen = true;

    // 파일이 없거나 비어 있어 읽은 시말서가 없는 경우
    if (list.empty()) {
        clearScreen();
        putBlank(); putBlank();
        string msg1 = "조회할 시말서가 없습니다.";
        string msg2 = "(" + string(REPRIMAND_FILE) + " 파일을 확인하세요)";
        string hint = "[아무 키] 메뉴로 돌아가기";
        putLine(pad((SCREEN_WIDTH - dispWidth(msg1)) / 2) + "\033[1;93m" + msg1);
        putBlank();
        putLine(pad((SCREEN_WIDTH - dispWidth(msg2)) / 2) + DIM + msg2);
        putBlank(); putBlank();
        putLine(pad((SCREEN_WIDTH - dispWidth(hint)) / 2) + DIM + hint);
        endFrame();
        readKey();
        return;
    }

    clearScreen();                            // 목록 화면 진입 시 1회 전체 지우기
    while (inScreen) {
        // --- 목록 화면 ---
        drawReprimandList(list, sel);
        Key k = readKey();
        if (k == K_UP) {
            sel = (sel + (int)list.size() - 1) % list.size();
        }
        else if (k == K_DOWN) {
            sel = (sel + 1) % list.size();
        }
        else if (k == K_BACK || k == K_QUIT) {
            inScreen = false;                     // 메뉴로 복귀
        }
        else if (k == K_ENTER) {
            // --- 상세 화면 (목록과 줄 수가 달라 전환 시 한 번 지움) ---
            clearScreen();
            bool inDetail = true;
            while (inDetail) {
                drawReprimandDetail(list[sel]);
                Key dk = readKey();
                if (dk == K_BACK || dk == K_QUIT || dk == K_ENTER)
                    inDetail = false;             // 목록으로 복귀
            }
            clearScreen();                        // 상세 -> 목록 복귀 시 1회 지우기
        }
    }
}

// ===================================================================
//  메인 메뉴 실행
// ===================================================================
int runTetrisMaster() {
    initConsole();
    cout << "\033[?25l";              // 커서 숨김
    runTruckAnimation();              // 청소차 한 번 지나감

    const vector<string> items = { "SINGLE", "2P", "시말서 조회"};
    int sel = 0, chosen = -1;

    clearScreen();                        // 메뉴 화면 진입 시 1회 전체 지우기
    bool running = true;
    while (running) {
        drawTitleAndMenu(items, sel);     // 이후엔 덮어쓰기 방식으로 갱신
        switch (readKey()) {
        case K_UP:    sel = (sel + (int)items.size() - 1) % items.size(); break;
        case K_DOWN:  sel = (sel + 1) % items.size();                     break;
        case K_QUIT:  chosen = -1; running = false;                       break;
        case K_ENTER:
            if (sel == 2) {
                runReprimandScreen();   // 시말서 조회 -> 화면 진입 후 메뉴 복귀
                clearScreen();          // 돌아오면 메뉴 다시 그리기 전 1회 지우기
            }
            else if (sel == 3) {
                writeReprimand();       // 시말서 작성 -> 입력/저장 후 메뉴 복귀
                clearScreen();
            }
            else {
                chosen = sel; running = false;   // SINGLE/2P -> 선택 확정
            }
            break;
        default: break;
        }
    }

    cout << "\033[?25h";              // 커서 복원
    cout << RESET;                    // 색 속성 초기화
    cout.flush();
    clearScreen();                    // ANSI 클리어
    hardClearConsole();               // 콘솔 버퍼까지 확실히 비워 잔상 제거
    return chosen;                    // 0=SINGLE, 1=2P, -1=종료
}