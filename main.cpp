#include <Novice.h>
#include <cmath>

struct Matrix4x4 {
	float m[4][4];
};

// 透視投影行列
Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip) {

	Matrix4x4 resultPerspectiveFov = {};

	resultPerspectiveFov.m[0][0] = (1 / aspectRatio) * (1 / tanf(fovY / 2));
	resultPerspectiveFov.m[1][1] = 1 / tanf(fovY / 2);
	resultPerspectiveFov.m[2][2] = farClip / (farClip - nearClip);
	resultPerspectiveFov.m[2][3] = 1.0f;
	resultPerspectiveFov.m[3][2] = -nearClip * farClip / (farClip - nearClip);

	return resultPerspectiveFov;
}

// 正射影行列
Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip) {

	Matrix4x4 resultOrthographic = {};

	resultOrthographic.m[0][0] = 2 / (right - left);
	resultOrthographic.m[1][1] = 2 / (top - bottom);
	resultOrthographic.m[2][2] = 1 / (farClip - nearClip);
	resultOrthographic.m[3][0] = (left + right) / (left - right);
	resultOrthographic.m[3][1] = (top + bottom) / (bottom - top);
	resultOrthographic.m[3][2] = nearClip / (nearClip - farClip);
	resultOrthographic.m[3][3] = 1.0f;

	return resultOrthographic;
}

// ビューポート変換行列
Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth) {

	Matrix4x4 resultViewport = {};

	resultViewport.m[0][0] = width / 2;
	resultViewport.m[1][1] = -height / 2;
	resultViewport.m[2][2] = maxDepth - minDepth;
	resultViewport.m[3][0] = left + (width / 2);
	resultViewport.m[3][1] = top + (height / 2);
	resultViewport.m[3][2] = minDepth;
	resultViewport.m[3][3] = 1.0f;

	return resultViewport;
}

static const int kRowHeight = 20;
static const int kColumnWidth = 60;

// 4x4行列の数値表示
void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix) {
	for (int row = 0; row < 4; ++row) {
		for (int column = 0; column < 4; ++column) {
			Novice::ScreenPrintf(x + column * kColumnWidth, y + row * kRowHeight, "%6.02f", matrix.m[row][column]);
		}
	}
}

const char kWindowTitle[] = "LE2B_01_アキモト_ワタル";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///

		Matrix4x4 orthographicMatarix =
			MakeOrthographicMatrix(-160.0f, 160.0f, 200.0f, 300.0f, 0.0f, 1000.0f);

		Matrix4x4 perspectiveFovMatrix =
			MakePerspectiveFovMatrix(0.63f, 1.33f, 0.1f, 1000.0f);

		Matrix4x4 viewportMatrix =
			MakeViewportMatrix(100.0f, 200.0f, 600.0f, 300.0f, 0.0f, 1.0f);

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		MatrixScreenPrintf(0, 0, orthographicMatarix);
		MatrixScreenPrintf(0, kRowHeight * 5, perspectiveFovMatrix);
		MatrixScreenPrintf(0, kRowHeight * 5 * 2, viewportMatrix);

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
