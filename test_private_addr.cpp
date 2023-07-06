#include <cstdint>
#include <cstring>
#include <type_traits>

class Register {
    public:
        Register(volatile uint32_t* addr) : m_addr(addr) {}

        inline uint32_t read() { return *m_addr; }
        inline void write(uint32_t value) { *m_addr = value; }

        template<class FuncPtr, std::enable_if_t<std::is_invocable_v<FuncPtr, uint32_t>, bool> = false>
        inline void modify(FuncPtr mod_functor) { *m_addr = mod_functor(*m_addr); }

    private:
        volatile uint32_t* const m_addr;
};

class StatusRegister : Register {
    public:
        enum class State {
            Active,
            Idle,
            Fault
        };

        class Fields {
            public:
                Fields() { memset(&m_bits, 0, sizeof(m_bits)); }
                explicit Fields(uint32_t value) { memcpy(&m_bits, &value, sizeof(value)); }

                inline bool busy() const { return m_bits.busy; }
                inline State state() const { return static_cast<State>(m_bits.state); }
                inline bool frame_error() const { return m_bits.frame_error; }
                inline bool overflow_error() const { return m_bits.overflow_error; }
                inline void clear_overflow_error() { m_bits.overflow_error = 1; }
                inline void clear_frame_error() { m_bits.frame_error = 1; }

                inline uint32_t reg_value() const {
                    uint32_t value = 0;
                    memcpy(&value, &m_bits, sizeof(value));
                    return value;
                }

            private:
                struct {
                    uint32_t busy : 1;
                    uint32_t state : 2;
                    uint32_t : 3;
                    uint32_t overflow_error : 1;
                    uint32_t frame_error : 1;
                    uint32_t : 24;
                } m_bits;
        };

        StatusRegister(volatile uint32_t * addr) : Register(addr) {}
        Fields read() { return Fields{Register::read()}; }

        template <class CALLABLE, std::enable_if_t<std::is_invocable_v<CALLABLE, Fields&>, bool> = false>
        void write(CALLABLE callable) {
            Fields f;
            callable(f);
            Register::write(f.reg_value());
        }

        template <class CALLABLE, std::enable_if_t<std::is_invocable_v<CALLABLE, const Fields&, Fields&>, bool> = false>
        void modify(CALLABLE callable) {
            Register::modify([=](uint32_t r_val) {
                Fields r{r_val};
                Fields w;
                callable(r, w);
                return w.reg_value();
            });
        }
};

static uint32_t status_reg_mem = 0;

void clear_frame_error() {
    StatusRegister status_reg(&status_reg_mem);
    status_reg.modify([=](const auto &r, auto &w) {
        if (r.frame_error()) {
            w.clear_frame_error();
        }
    });
}

void clear_overflow_error() {
    StatusRegister status_reg(&status_reg_mem);
    status_reg.modify([=](const auto &r, auto &w) {
        if (r.overflow_error()) {
            w.clear_overflow_error();
        }
    });
}

bool is_busy() {
    StatusRegister status_reg(&status_reg_mem);
    return status_reg.read().busy();
}

auto get_state() {
    StatusRegister status_reg(&status_reg_mem);
    return status_reg.read().state();
}
