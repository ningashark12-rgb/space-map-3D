#include <windows.h>
#include <GL/gl.h>
#include <cstdlib>
#include <ctime>
#include <cmath>

const int NUM_STARS = 400;
float starPositions[NUM_STARS * 3];
int selectedStar = -1;

float camX = 0.0f, camY = 0.0f, camZ = 2.0f;
float camSpeed = 0.04f;

void generateStars() {
srand(time(0));
for (int i = 0; i < NUM_STARS * 3; i++) {
starPositions[i] = ((float)rand() / RAND_MAX) * 4.0f - 2.0f;
}
}

void handleMouseClick(int mouseX, int mouseY) {
float clickX = ((float)mouseX / 1024.0f) * 2.0f - 1.0f;
float clickY = -(((float)mouseY / 768.0f) * 2.0f - 1.0f);
float aspect = 1024.0f / 768.0f;
clickX *= aspect;

int closestIdx = -1;
float minDist = 9999.0f;

for (int i = 0; i < NUM_STARS; i++) {
float sx = starPositions[i * 3] - camX;
float sy = starPositions[i * 3 + 1] - camY;
float sz = starPositions[i * 3 + 2] - camZ;

if (sz >= 0) continue;

float projX = (sx / -sz);
float projY = (sy / -sz);

float d = sqrtf((clickX - projX)*(clickX - projX) + (clickY - projY)*(clickY - projY));
if (d < minDist && d < 0.1f) {
minDist = d;
closestIdx = i;
}
}
selectedStar = closestIdx;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
switch (uMsg) {
case WM_CLOSE: PostQuitMessage(0); return 0;
case WM_LBUTTONDOWN:
handleMouseClick(LOWORD(lParam), HIWORD(lParam));
return 0;
case WM_KEYDOWN: if (wParam == VK_ESCAPE) PostQuitMessage(0); return 0;
}
return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void processMovement() {
if (GetKeyState(VK_UP) & 0x8000) camZ -= camSpeed;
if (GetKeyState(VK_DOWN) & 0x8000) camZ += camSpeed;
if (GetKeyState(VK_LEFT) & 0x8000) camX -= camSpeed;
if (GetKeyState(VK_RIGHT) & 0x8000) camX += camSpeed;
}

void drawPaths() {
glLineWidth(0.5f);
glColor4f(0.2f, 0.4f, 0.8f, 0.3f);
glBegin(GL_LINES);
for (int i = 0; i < NUM_STARS; i++) {
float x1 = starPositions[i * 3]; float y1 = starPositions[i * 3 + 1]; float z1 = starPositions[i * 3 + 2];
for (int j = i + 1; j < NUM_STARS; j++) {
float x2 = starPositions[j * 3]; float y2 = starPositions[j * 3 + 1]; float z2 = starPositions[j * 3 + 2];
float dist = sqrtf((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) + (z1-z2)*(z1-z2));
if (dist < 0.4f) {
glVertex3f(x1, y1, z1);
glVertex3f(x2, y2, z2);
}
}
}
glEnd();
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
WNDCLASS wc = {}; wc.lpfnWndProc = WindowProc; wc.hInstance = hInstance; wc.lpszClassName = "SpaceMap3DWindow";
RegisterClass(&wc);
HWND hwnd = CreateWindowEx(0, wc.lpszClassName, "Space Map 3D", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 1024, 768, NULL, NULL, hInstance, NULL);
HDC hdc = GetDC(hwnd);
PIXELFORMATDESCRIPTOR pfd = { sizeof(PIXELFORMATDESCRIPTOR), 1, PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, PFD_TYPE_RGBA, 32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 24, 8, 0, PFD_MAIN_PLANE, 0, 0, 0, 0 };
int pf = ChoosePixelFormat(hdc, &pfd); SetPixelFormat(hdc, pf, &pfd);
HGLRC hglrc = wglCreateContext(hdc); wglMakeCurrent(hdc, hglrc);

generateStars();
glEnable(GL_DEPTH_TEST);
glEnable(GL_BLEND);
glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

MSG msg = {};
while (msg.message != WM_QUIT) {
if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
TranslateMessage(&msg); DispatchMessage(&msg);
} else {
processMovement();
glClearColor(0.03f, 0.03f, 0.07f, 1.0f);
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

glMatrixMode(GL_PROJECTION); glLoadIdentity();
float aspect = 1024.0f / 768.0f;
glFrustum(-aspect * 0.1f, aspect * 0.1f, -0.1f, 0.1f, 0.1f, 100.0f);

glMatrixMode(GL_MODELVIEW); glLoadIdentity();
glTranslatef(-camX, -camY, -camZ);

drawPaths();

// Draw all base background stars
glPointSize(4.0f);
glColor3f(1.0f, 1.0f, 1.0f);
glBegin(GL_POINTS);
for (int i = 0; i < NUM_STARS; i++) {
if (i != selectedStar) {
glVertex3f(starPositions[i * 3], starPositions[i * 3 + 1], starPositions[i * 3 + 2]);
}
}
glEnd();

// Draw the target selected star on top with a massive size
if (selectedStar != -1) {
glPointSize(12.0f);
glColor3f(0.0f, 1.0f, 0.0f);
glBegin(GL_POINTS);
glVertex3f(starPositions[selectedStar * 3], starPositions[selectedStar * 3 + 1], starPositions[selectedStar * 3 + 2]);
glEnd();
}

SwapBuffers(hdc);
}
}
wglMakeCurrent(NULL, NULL); wglDeleteContext(hglrc); ReleaseDC(hwnd, hdc); DestroyWindow(hwnd);
return 0;
}

