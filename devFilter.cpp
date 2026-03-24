#include <iostream>
#include <cstring> // dùng memset

enum device {
    DevA = 0,
    DevB,
    DevC,
    DevCount,
};

unsigned short devInMachine[] = {
    0, 1, 4, 6, 7, 8, 11
};

static constexpr unsigned short DEV = DevB; // Device cần loại bỏ

class A {
public:
    static void print();
    static void filter(unsigned short *arr, unsigned short start, unsigned short end, int &validSize);

    static constexpr unsigned short DEVNUM_A = 1;
    static constexpr unsigned short DEVNUM_B = 1;
    static constexpr unsigned short DEVNUM_C = 1;
    static constexpr unsigned short TOTALDEVNUM = DEVNUM_A + DEVNUM_B + DEVNUM_C;

    static const unsigned short devNum[DevCount];
    static const unsigned short dev[TOTALDEVNUM];
};

const unsigned short A::devNum[DevCount] = {DEVNUM_A, DEVNUM_B, DEVNUM_C};
const unsigned short A::dev[TOTALDEVNUM] = {0, 2, 3};

void A::print() {
    static bool init = false;
    static unsigned short removeDev[TOTALDEVNUM];
    static int stIdx = 0;
    static int enIdx = 0;
    static int validSize = 0;

    if (!init) {
        // Khởi tạo tất cả phần tử bằng 0xFFFF (memset gán theo byte)
        memset(removeDev, 0xFF, sizeof(removeDev));

        // Tính toán vị trí bắt đầu và kết thúc cho device cần loại bỏ
        stIdx = 0;
        for (int i = 0; i < DEV; i++) {
            stIdx += devNum[i];
        }
        enIdx = stIdx + devNum[DEV] - 1;
        init = true;

        // Gọi filter để loại bỏ device được chọn
        filter(removeDev, stIdx, enIdx, validSize);

        std::cout << "Filtered dev list (removed Dev " << DEV << "): ";
        for (int i = 0; i < validSize; i++) {
            std::cout << removeDev[i] << " ";
        }
        std::cout << std::endl;
    }

    // In devInMachine nhưng bỏ qua phần tử nào trùng với removeDev
    std::cout << "devInMachine (filtered): ";
    for (unsigned short val : devInMachine) {
        bool skip = false;
        for (int j = 0; j < validSize; j++) {
            if (val == removeDev[j]) {
                skip = true;
                break;
            }
        }
        if (!skip) {
            std::cout << val << " ";
        }
    }
    std::cout << std::endl;
}

void A::filter(unsigned short *arr, unsigned short start, unsigned short end, int &validSize) {
    validSize = 0;
    for (int i = 0; i < TOTALDEVNUM; i++) {
        if (i >= start && i <= end) {
            continue; // bỏ qua các phần tử thuộc device cần loại bỏ
        }
        arr[validSize++] = dev[i];
    }
}

int main() {
    std::cout << "Hello World" << std::endl;
    A::print();
    return 0;
}
