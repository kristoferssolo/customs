#include <cstdint>
#include <stdexcept>
constexpr char FILENAME_IN[] = "customs.in";
constexpr char FILENAME_OUT[] = "customs.out";
constexpr uint8_t LINE_LENGTH = 20;
using Time = uint32_t;
using CustomsCount = uint8_t;

template <typename T> void swap(T *a, T *b) {
    T temp = *a;
    *a = *b;
    *b = temp;
}

template <typename T> struct Vec {
 public:
    void push(const T &value) {
        if (m_size == m_capacity) {
            reserve_exact(m_size == 0 ? 1 : 2 * m_size);
        }
        data[m_size++] = value;
    }

    T pop() {
        if (is_empty()) {
            throw std::out_of_range("Cannot pop from empty Vec");
        }
        return data[m_size--];
    }

    T first() {
        if (is_empty()) {
            throw std::out_of_range("Cannot get first from empty Vec");
        }
        return data[0];
    }

    T last() {
        if (is_empty()) {
            throw std::out_of_range("Cannot get last from empty Vec");
        }
        return data[m_size - 1];
    }

    size_t len() const {
        return m_size;
    }

    size_t capacity() const {
        return m_capacity;
    }

    bool is_empty() const {
        return m_size == 0;
    }

    void clear() {
        m_size = 0;
    }

    bool contains(const T &value) const {
        for (size_t i = 0; i < m_size; ++i) {
            if (data[i] == value) {
                return true;
            }
        }
        return false;
    }

    T first() const {
        if (is_empty()) {
            throw std::out_of_range("Cannot get first from empty Vec");
        }
        return data[0];
    }

    T &operator[](const size_t idx) {
        if (idx >= m_size) {
            throw std::out_of_range("Index out of range");
        }
        return data[idx];
    }

    const T &operator[](const size_t idx) const {
        if (idx >= m_size) {
            throw std::out_of_range("Index out of range");
        }
        return data[idx];
    }

    T get(const size_t idx) const {
        if (idx >= m_size) {
            throw std::out_of_range("Index out of range");
        }
        return data[idx];
    }

    Vec<T> reserve_exact(size_t additional) {
        if (additional > m_capacity) {
            T *new_data = new T[additional];
            for (size_t i = 0; i < m_size; ++i) {
                new_data[i] = data[i];
            }
            delete[] data;
            data = new_data;
            m_capacity = additional;
        }
        return *this;
    }

    static Vec<T> with_capacity(size_t capacity) {
        auto vec = Vec<T>::create();
        vec.reserve_exact(capacity);
        return vec;
    }

    static Vec<T> create() {
        return {};
    }

    class Iterator {
     public:
        explicit Iterator(T *ptr): ptr(ptr) {
        }

        Iterator &operator++() {
            ++ptr;
            return *this;
        }

        bool operator!=(const Iterator &other) const {
            return ptr != other.ptr;
        }

        T &operator*() const {
            return *ptr;
        }

     private:
        T *ptr;
    };

    Iterator begin() const {
        return Iterator(data);
    }

    Iterator end() const {
        return Iterator(data + m_size);
    }

    Vec(): data(nullptr), m_capacity(0), m_size(0) {
    }

    T *data;

 protected:
    size_t m_capacity;
    size_t m_size;
};

template <typename T> struct BinaryHeap {
    BinaryHeap(): data(Vec<T>::create()) {
        data.push(T());
    }

    static BinaryHeap<T> create() {
        return {};
    }

    void push(const T &value) {
        data.push(value);
        sift_up(len());
    }

    T pop() {
        if (is_empty()) {
            throw std::out_of_range("Cannot pop from empty BinaryHeap");
        }
        T first = data[1];

        swap(&data[len()], &data[1]);
        data.pop();

        sift_down(1);
        return first;
    }

    T *peek() {
        return &data[1];
    }

    void clear() {
        data.clear();
        data.push(T());
    }

    Vec<T> *into_vec() {
        return &data;
    }

    size_t len() const {
        return data.len() - 1;
    }

    size_t capacity() const {
        return data.capacity() - 1;
    }

    bool is_empty() const {
        return len() == 0;
    }

    static BinaryHeap<T> from(const Vec<T> &data) {
        BinaryHeap<T> heap = BinaryHeap<T>::create();
        for (const auto &value : data) {
            heap.push(value);
        }
        return heap;
    }

    static BinaryHeap<T> with_capacity(const size_t capacity) {
        BinaryHeap<T> heap = BinaryHeap<T>::create();
        heap.data = Vec<T>::with_capacity(capacity);
        heap.push(T());
        return heap;
    }

    class Iterator {
     public:
        explicit Iterator(T *ptr): ptr(ptr) {
        }

        Iterator &operator++() {
            ++ptr;
            return *this;
        }

        bool operator!=(const Iterator &other) const {
            return ptr != other.ptr;
        }

        T &operator*() const {
            return *ptr;
        }

     private:
        T *ptr;
    };

    Iterator begin() const {
        return Iterator(data.data + 1);
    }

    Iterator end() const {
        return Iterator(data.data + data.len());
    }

    T &operator[](const size_t idx) {
        if (idx >= len()) {
            throw std::out_of_range("Index out of range");
        }
        return data[idx];
    }

    const T &operator[](const size_t idx) const {
        if (idx >= len()) {
            throw std::out_of_range("Index out of range");
        }
        return data[idx];
    }

    void sift_up(size_t idx) {
        if (idx <= 1) {
            return;
        }
        size_t parent_idx = idx / 2;
        if (data[idx] < data[parent_idx]) {
            swap(&data[idx], &data[parent_idx]);
            sift_up(parent_idx);
        }
    }

    void sift_down(size_t idx) {
        size_t smallest = idx;
        size_t left_child = idx * 2;
        size_t right_child = idx * 2 + 1;
        size_t size = data.len();

        if (left_child < size && data[left_child] < data[smallest]) {
            smallest = left_child;
        }
        if (right_child < size && data[right_child] < data[smallest]) {
            smallest = right_child;
        }

        if (smallest != idx) {
            swap(&data[idx], &data[smallest]);
            sift_down(smallest);
        }
    }

 protected:
    Vec<T> data;
};

enum class Citizenship {
    Citizen,
    NonCitizen,
    Undefined,
};

namespace citizenshipType {
    Citizenship from_str(const char &ch) {
        switch (ch) {
            case 'P': return Citizenship::Citizen;
            case 'N': return Citizenship::NonCitizen;
            default: return Citizenship::Undefined;
        }
    }

    const char *to_string(const Citizenship type) {
        switch (type) {
            case Citizenship::Citizen: return "Citizen";
            case Citizenship::NonCitizen: return "NonCitizen";
            case Citizenship::Undefined: return "Undefined";
        }
        return nullptr;
    }
}  // namespace citizenshipType

struct Traveler {
    Time arrival_time;
    Citizenship citizenship;
    Time exit_time = 0;
    CustomsCount officer_id = 0;

    Traveler(): arrival_time(0), citizenship(Citizenship::Undefined) {
    }

    static Traveler create(Time arrival_time, Citizenship type) {
        return {arrival_time, type};
    }

    static Traveler from_str(const char *string) {
        char t;
        Time arrival_time;
        sscanf(string, "%s %u", &t, &arrival_time);
        Citizenship type = citizenshipType::from_str(t);
        return {arrival_time, type};
    }

    bool operator==(const Traveler &other) const {
        return arrival_time == other.arrival_time && citizenship == other.citizenship && exit_time == other.exit_time &&
               officer_id == other.officer_id;
    }

    bool operator<(const Traveler &other) const {
        if (exit_time != other.exit_time) {
            return exit_time < other.exit_time;
        }
        if (citizenship != other.citizenship) {
            return citizenship < other.citizenship;
        }
        return officer_id < other.officer_id;
    }

    bool operator!=(const Traveler &other) const {
        return !(*this == other);
    }

    bool operator>(const Traveler &other) {
        return other < *this;
    }

    bool operator<=(const Traveler &other) const {
        return !(other < *this);
    }

    bool operator>=(const Traveler &other) const {
        return !(*this < other);
    }

    bool is_wait_time_exceede(Time exit_time, Time max_wait_time) {
        return (exit_time - this->exit_time) > max_wait_time + 1;
    }

    void print() {
        printf("%d %d\n", arrival_time, exit_time);
    }

 protected:
    Traveler(Time arrival_time, Citizenship type): arrival_time(arrival_time), citizenship(type) {
    }
};

struct Officer {
    CustomsCount id;
    Time processing_time;
    Time departure_time = 1;
    Citizenship citizenship;

    Officer(): id(0), processing_time(0), citizenship(Citizenship::Undefined) {
    }

    static Officer create(CustomsCount id, Time processing_time, Citizenship citizenship) {
        return {id, processing_time, citizenship};
    }

    Time total_time() const {
        return processing_time + departure_time;
    }

    static Officer from_str(const char *string) {
        char t, type;
        CustomsCount customs_id;
        Time processing_time;

        sscanf(string, "%s %s %hhu %u", &t, &type, &customs_id, &processing_time);

        Citizenship citizenship = citizenshipType::from_str(type);
        return {customs_id, processing_time, citizenship};
    }

    bool operator==(const Officer &other) const {
        return id == other.id && processing_time == other.processing_time && departure_time == other.departure_time &&
               citizenship == other.citizenship;
    }

    bool operator<(const Officer &other) const {
        if (departure_time != other.departure_time) {
            return departure_time < other.departure_time;
        }
        return id < other.id;
    }

    bool operator!=(const Officer &other) const {
        return !(*this == other);
    }

    bool operator>(const Officer &other) {
        return other < *this;
    }

    bool operator<=(const Officer &other) const {
        return !(other < *this);
    }

    bool operator>=(const Officer &other) const {
        return !(*this < other);
    }

 private:
    Officer(CustomsCount id, Time processing_time, Citizenship citizenship):
        id(id),
        processing_time(processing_time),
        citizenship(citizenship) {
    }
};

struct DefaultOfficer {
    CustomsCount count;
    Time time;
    Citizenship citizenship;

    static DefaultOfficer create(const CustomsCount count, const Time time, const Citizenship citizenship) {
        return {count, time, citizenship};
    }

 private:
    DefaultOfficer(const CustomsCount count, const Time time, const Citizenship citizenship):
        count(count),
        time(time),
        citizenship(citizenship) {
    }
};

struct CustomsInfo {
    DefaultOfficer citizen;
    DefaultOfficer non_citizen;

    static CustomsInfo from_str(const char *string) {
        Time citizen_time, non_citizen_time;
        CustomsCount citizen_count, non_citizen_count;

        sscanf(string, "%hhu %hhu %u %u", &citizen_count, &non_citizen_count, &citizen_time, &non_citizen_time);

        return {DefaultOfficer::create(citizen_count, citizen_time, Citizenship::Citizen),
                DefaultOfficer::create(non_citizen_count, non_citizen_time, Citizenship::NonCitizen)};
    }

 protected:
    CustomsInfo(const DefaultOfficer &citizen, const DefaultOfficer &non_citizen):
        citizen(citizen),
        non_citizen(non_citizen) {
    }
};

struct CustomsVec {
    Vec<Officer> citizens;
    Vec<Officer> non_citizens;

    static CustomsVec from(const CustomsInfo &value) {
        auto self = CustomsVec::create();
        self.citizens = Vec<Officer>::with_capacity(value.citizen.count);
        self.set_default_times(value.citizen, Citizenship::Citizen);

        self.non_citizens = Vec<Officer>::with_capacity(value.non_citizen.count);
        self.set_default_times(value.non_citizen, Citizenship::NonCitizen);

        return self;
    }

    void push(const Officer &officer) {
        switch (officer.citizenship) {
            case Citizenship::Citizen: citizens.push(officer); break;
            case Citizenship::NonCitizen: non_citizens.push(officer); break;
            case Citizenship::Undefined: break;
        }
    }

    bool is_empty() const {
        return citizens.is_empty() && non_citizens.is_empty();
    }

    void replace(const Officer &officer) {
        switch (officer.citizenship) {
            case Citizenship::Citizen: citizens[officer.id - 1] = officer; break;
            case Citizenship::NonCitizen: non_citizens[officer.id - 1] = officer; break;
            case Citizenship::Undefined: break;
        }
    }

    void set_default_times(const DefaultOfficer &default_officer, const Citizenship citizenship) {
        for (CustomsCount customs_id = 1; customs_id <= default_officer.count; ++customs_id) {
            push(Officer::create(customs_id, default_officer.time, citizenship));
        }
    }

    static CustomsVec create() {
        return {};
    }

 protected:
    CustomsVec(): citizens(Vec<Officer>::create()), non_citizens(Vec<Officer>::create()) {
    }
};

struct CustomsBinaryHeap {
    BinaryHeap<Officer> citizens;
    BinaryHeap<Officer> non_citizens;

    static CustomsBinaryHeap create() {
        return {};
    }

    void push(Officer officer) {
        switch (officer.citizenship) {
            case Citizenship::Citizen: citizens.push(officer); break;
            case Citizenship::NonCitizen: non_citizens.push(officer); break;
            case Citizenship::Undefined: break;
        }
    }

    bool is_empty() const {
        return citizens.is_empty() && non_citizens.is_empty();
    }

    void process(Traveler *traveler) {
        BinaryHeap<Officer> customs_heap;
        switch (traveler->citizenship) {
            case Citizenship::Citizen: customs_heap = citizens; break;
            case Citizenship::NonCitizen: customs_heap = non_citizens; break;
            case Citizenship::Undefined: throw std::logic_error("Unexpected citizen type");
        }
        if (customs_heap.is_empty()) {
            throw std::logic_error("Unexpected empty heap");
        }
        Officer officer = customs_heap.pop();
        traveler->exit_time = officer.total_time();
        officer.departure_time = officer.total_time();
        traveler->officer_id = officer.id;
        customs_heap.push(officer);
    }

    static CustomsBinaryHeap from(const CustomsVec &data) {
        auto self = CustomsBinaryHeap::create();
        self.citizens = BinaryHeap<Officer>::from(data.citizens);
        self.non_citizens = BinaryHeap<Officer>::from(data.non_citizens);
        return self;
    }

    void update_departure_times(const Traveler &traveler) {
        Time time = traveler.arrival_time;
        size_t idx = 1;
        BinaryHeap<Officer> customs;
        switch (traveler.citizenship) {
            case Citizenship::Citizen: customs = citizens; break;
            case Citizenship::NonCitizen: customs = non_citizens; break;
            case Citizenship::Undefined: return;
        }
        for (auto &officer : customs) {
            if (officer.departure_time < time) {
                officer.departure_time = time;
                customs.sift_up(idx);
                ++idx;
            }
        }
    }

 protected:
    CustomsBinaryHeap(): citizens(BinaryHeap<Officer>::create()), non_citizens(BinaryHeap<Officer>::create()) {
    }
};

char *get_line(char *string) {
    return fgets(string, LINE_LENGTH, stdin);
}

Time max_time(const Time a, const Time b) {
    return (a > b) ? a : b;
}

bool parse_input() {
    char *line = new char[LINE_LENGTH];
    CustomsInfo first_line = CustomsInfo::from_str(get_line(line));

    auto customs_vec = CustomsVec::from(first_line);

    size_t max_wait_time = max_time(first_line.citizen.time, first_line.non_citizen.time);

    bool output_exists = false;

    while (get_line(line)[0] != 'X') {
        if (line[0] != 'T') {
            break;
        }
        // Replace officers with non default values
        Officer officer = Officer::from_str(line);
        customs_vec.replace(officer);
        max_wait_time = max_time(max_wait_time, officer.processing_time);
    }
    auto customs_heap = CustomsBinaryHeap::from(customs_vec);
    auto travelers_heap = BinaryHeap<Traveler>::with_capacity(10);

    for (; line[0] != 'X'; get_line(line)) {
        Traveler traveler = Traveler::from_str(line);
        customs_heap.update_departure_times(traveler);
        customs_heap.process(&traveler);
        travelers_heap.push(traveler);

        if (travelers_heap.peek()->is_wait_time_exceede(traveler.exit_time, max_wait_time)) {
            travelers_heap.pop().print();
            output_exists = true;
        }
    }

    while (travelers_heap.len() > 0) {
        travelers_heap.pop().print();
        output_exists = true;
    }
    delete[] line;
    return output_exists;
}

void process() {
    if (!parse_input()) {
        printf("nothing\n");
    }
}

int main() {
    freopen(FILENAME_IN, "r", stdin);
    freopen(FILENAME_OUT, "w", stdout);

    process();

    fclose(stdin);
    fclose(stdout);
}
