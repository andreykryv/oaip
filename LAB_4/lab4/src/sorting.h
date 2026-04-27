#pragma once
#include <vector>
#include <algorithm>
#include <cstring>

// ═══════════════════════════════════════════════════════════
//  Sorting Algorithms (Задание 1 & 2)
// ═══════════════════════════════════════════════════════════

// ── Heap Sort ─────────────────────────────────────────────
namespace detail {
    inline void heapify(int* arr, int n, int i) {
        int largest = i, l = 2*i+1, r = 2*i+2;
        if (l < n && arr[l] > arr[largest]) largest = l;
        if (r < n && arr[r] > arr[largest]) largest = r;
        if (largest != i) {
            std::swap(arr[i], arr[largest]);
            heapify(arr, n, largest);
        }
    }

    inline void mergeParts(int* arr, int l, int m, int r) {
        int n1 = m-l+1, n2 = r-m;
        int* L = new int[n1]; int* R = new int[n2];
        for (int i=0;i<n1;++i) L[i]=arr[l+i];
        for (int j=0;j<n2;++j) R[j]=arr[m+1+j];
        int i=0,j=0,k=l;
        while (i<n1&&j<n2) arr[k++] = L[i]<=R[j] ? L[i++] : R[j++];
        while (i<n1) arr[k++]=L[i++];
        while (j<n2) arr[k++]=R[j++];
        delete[] L; delete[] R;
    }
}

inline void heapSort(int* arr, int n) {
    for (int i=n/2-1;i>=0;--i) detail::heapify(arr,n,i);
    for (int i=n-1;i>=0;--i) { std::swap(arr[0],arr[i]); detail::heapify(arr,i,0); }
}

// ── Quick Sort ────────────────────────────────────────────
namespace detail {
    inline int partition(int* arr, int lo, int hi) {
        int pivot = arr[hi], i = lo-1;
        for (int j=lo;j<hi;++j)
            if (arr[j]<=pivot) { ++i; std::swap(arr[i],arr[j]); }
        std::swap(arr[i+1],arr[hi]);
        return i+1;
    }
    inline void qs(int* arr, int lo, int hi) {
        if (lo<hi) { int p=partition(arr,lo,hi); qs(arr,lo,p-1); qs(arr,p+1,hi); }
    }
}
inline void quickSort(int* arr, int n) { if (n>1) detail::qs(arr,0,n-1); }

// ── Merge Sort ────────────────────────────────────────────
namespace detail {
    inline void ms(int* arr, int l, int r) {
        if (l<r) { int m=l+(r-l)/2; ms(arr,l,m); ms(arr,m+1,r); mergeParts(arr,l,m,r); }
    }
}
inline void mergeSort(int* arr, int n) { if (n>1) detail::ms(arr,0,n-1); }

// ── Interpolation Sort (with step recording) ─────────────
//   Insertion sort where insertion position is estimated
//   via linear interpolation on the already-sorted prefix.
inline void interpolationSort(int* arr, int n,
    std::vector<std::vector<int>>& steps)
{
    steps.clear();
    steps.emplace_back(arr, arr + n);   // шаг 0: исходный массив

    for (int i = 1; i < n; ++i) {
        int key = arr[i];

        // Оцениваем позицию key в отсортированном префиксе [0..i-1]
        // с помощью линейной интерполяции
        int pos = i;
        if (i >= 1) {
            if (key <= arr[0]) {
                pos = 0;
            } else if (key >= arr[i-1]) {
                pos = i;
            } else {
                // Интерполяция: pos = lo + (key - arr[lo]) * (hi - lo) / (arr[hi] - arr[lo])
                double t = static_cast<double>(key - arr[0]) /
                           (arr[i-1] - arr[0]);
                pos = static_cast<int>(t * (i-1));
                pos = std::max(0, std::min(i, pos));
                // Уточнение линейным смещением
                while (pos < i && arr[pos] < key)  ++pos;
                while (pos > 0 && arr[pos-1] > key) --pos;
            }
        }

        // Сдвигаем элементы вправо
        for (int j = i; j > pos; --j) arr[j] = arr[j-1];
        arr[pos] = key;

        steps.emplace_back(arr, arr + n);   // шаг i
    }
}

// ── Binary Search ─────────────────────────────────────────
inline int binsearch(const int* arr, int n, int digit) {
    int lo = 0, hi = n - 1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (arr[mid] == digit) return mid;
        else if (arr[mid] < digit) lo = mid + 1;
        else hi = mid - 1;
    }
    return -1;
}

// ── Binpow (быстрое возведение в степень по модулю) ───────
inline long long binpow(long long base, long long exp, long long mod) {
    if (mod == 1) return 0;
    long long result = 1;
    base %= mod;
    while (exp > 0) {
        if (exp & 1) result = result * base % mod;
        base = base * base % mod;
        exp >>= 1;
    }
    return result;
}

// ── Median of triplets (Задание 3) ───────────────────────
inline std::vector<double> medianTriplets(const std::vector<int>& a) {
    std::vector<double> b;
    int n = static_cast<int>(a.size());
    for (int i = 0; i < n; ) {
        int rem = std::min(3, n - i);
        if (rem == 3) {
            int tmp[3] = {a[i], a[i+1], a[i+2]};
            std::sort(tmp, tmp+3);
            b.push_back(static_cast<double>(tmp[1]));
        } else if (rem == 2) {
            b.push_back((a[i] + a[i+1]) / 2.0);
        } else {
            b.push_back(static_cast<double>(a[i]));
        }
        i += rem;
    }
    return b;
}
