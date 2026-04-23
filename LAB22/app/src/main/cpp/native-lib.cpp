#include <jni.h>
#include <string>
#include <vector>
#include <android/log.h>

#define TAG "LAB22_NATIVE"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__)

extern "C" {

// =======================================================
// 1) Hello World natif
// =======================================================
JNIEXPORT jstring JNICALL
Java_com_example_lab22_MainActivity_helloNative(JNIEnv* env, jobject) {
    LOGI("Appel de helloFromJNI depuis le natif");
    std::string msg = "Hello from Native C++ ";
    return env->NewStringUTF(msg.c_str());
}

// =======================================================
// 2) Factoriel avec gestion d'erreur
// =======================================================
JNIEXPORT jlong JNICALL
Java_com_example_lab22_MainActivity_factorial(JNIEnv* env, jobject, jint n) {

    if (n < 0) {
        jclass exception = env->FindClass("java/lang/IllegalArgumentException");
        env->ThrowNew(exception, "n doit être >= 0");
        return -1;
    }

    long result = 1;
    for (int i = 1; i <= n; i++) {
        result *= i;
    }

    LOGI("Factoriel de %d calcule en natif = %ld", n, result);

    return result;
}

// =======================================================
// 3) Inversion d'une chaîne Java → C++ → Java
// =======================================================
JNIEXPORT jstring JNICALL
Java_com_example_lab22_MainActivity_reverseString(JNIEnv* env, jobject, jstring input) {

    const char* str = env->GetStringUTFChars(input, nullptr);

    std::string s(str);
    std::reverse(s.begin(), s.end());

    LOGI("String inversee = %s", s.c_str());

    env->ReleaseStringUTFChars(input, str);

    return env->NewStringUTF(s.c_str());
}

// =======================================================
// 4) Somme d’un tableau int[]
// =======================================================
JNIEXPORT jint JNICALL
Java_com_example_lab22_MainActivity_sumArray(JNIEnv* env, jobject, jintArray arr) {

    jint* elements = env->GetIntArrayElements(arr, nullptr);
    jsize length = env->GetArrayLength(arr);

    int sum = 0;
    for (int i = 0; i < length; i++) {
        sum += elements[i];
    }

    LOGI("Somme du tableau = %d", sum);

    env->ReleaseIntArrayElements(arr, elements, 0);

    return sum;
}

// =======================================================
// 5) Multiplication matricielle
// =======================================================
JNIEXPORT jobjectArray JNICALL
Java_com_example_lab22_MainActivity_multiplyMatrices(JNIEnv* env, jobject,
                                                     jobjectArray A,
                                                     jobjectArray B) {

    int rowsA = env->GetArrayLength(A);
    int colsA = env->GetArrayLength((jintArray) env->GetObjectArrayElement(A, 0));

    int rowsB = env->GetArrayLength(B);
    int colsB = env->GetArrayLength((jintArray) env->GetObjectArrayElement(B, 0));

    // Vérification compatibilité
    if (colsA != rowsB) {
        jclass exception = env->FindClass("java/lang/IllegalArgumentException");
        env->ThrowNew(exception, "Matrices incompatibles");
        return nullptr;
    }

    // Classe int[]
    jclass intArrayClass = env->FindClass("[I");

    // Résultat
    jobjectArray result = env->NewObjectArray(rowsA, intArrayClass, nullptr);

    for (int i = 0; i < rowsA; i++) {
        jintArray rowA = (jintArray) env->GetObjectArrayElement(A, i);
        jint* elemsA = env->GetIntArrayElements(rowA, nullptr);

        jintArray resultRow = env->NewIntArray(colsB);
        std::vector<jint> rowResult(colsB, 0);

        for (int j = 0; j < colsB; j++) {
            for (int k = 0; k < colsA; k++) {
                jintArray rowB = (jintArray) env->GetObjectArrayElement(B, k);
                jint* elemsB = env->GetIntArrayElements(rowB, nullptr);

                rowResult[j] += elemsA[k] * elemsB[j];

                env->ReleaseIntArrayElements(rowB, elemsB, 0);
            }
        }

        env->SetIntArrayRegion(resultRow, 0, colsB, rowResult.data());
        env->SetObjectArrayElement(result, i, resultRow);

        env->ReleaseIntArrayElements(rowA, elemsA, 0);
    }

    LOGI("Produit matriciel calcule en natif");

    return result;
}

}