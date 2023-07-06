#include <cstdint>
#include <cstring>
#include <type_traits>

template <volatile uint32_t * ADDR>
class Register {
    public:
        Register() {}

        inline uint32_t read() {
            return *ADDR;
        }

        inline void write(uint32_t value) {
            *ADDR = value;
        }

        template<class CALLABLE, std::enable_if_t<std::is_invocable_v<CALLABLE, uint32_t>, bool> = false>
        inline void modify(CALLABLE callable) {
            *ADDR = callable(*ADDR);
        }
};

template <volatile uint32_t * ADDR>
class StatusRegister : Register<ADDR> {
    public:
        enum class State {
            Active,
            Idle,
            Fault
        };

        class Fields {
            public:
                Fields() {
                    memset(&m_bits, 0, sizeof(m_bits));
                }

                explicit Fields(uint32_t value) {
                    memcpy(&m_bits, &value, sizeof(value));
                }

                inline bool busy() const { return m_bits.busy; }
                inline State state() const { return static_cast<State>(m_bits.state); }
                inline bool frame_error() const { return m_bits.frame_error; }
                inline bool overflow_error() const { return m_bits.ovfl_error; }
                inline void clear_overflow_error() { m_bits.ovfl_error = 1; }
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
                    uint32_t ovfl_error : 1;
                    uint32_t frame_error : 1;
                    uint32_t : 24;
                } m_bits;
        };

        StatusRegister() : Register<ADDR>() {}
        Fields read() { return Fields{Register<ADDR>::read()}; }

        template <class CALLABLE, std::enable_if_t<std::is_invocable_v<CALLABLE, Fields&>, bool> = false>
        void write(CALLABLE callable) {
            Fields f;
            callable(f);
            Register<ADDR>::write(f.reg_value());
        }

        template <class CALLABLE, std::enable_if_t<std::is_invocable_v<CALLABLE, Fields&, Fields&>, bool> = false>
        void modify(CALLABLE callable) {
            Register<ADDR>::modify([=](uint32_t r_val) {
                Fields r{r_val};
                Fields w;
                callable(r, w);
                return w.reg_value();
            });
        }
};

static uint32_t status_reg_mem = 0;

void clear_frame_error() {
    StatusRegister<&status_reg_mem> status_reg;
    status_reg.modify([=](const auto &r, auto &w) {
        if (r.frame_error()) {
            w.clear_frame_error();
        }
    });
}

void clear_overflow_error() {
    StatusRegister<&status_reg_mem> status_reg;
    status_reg.modify([=](const auto &r, auto &w) {
        if (r.overflow_error()) {
            w.clear_overflow_error();
        }
    });
}

bool is_busy() {
    StatusRegister<&status_reg_mem> status_reg;
    return status_reg.read().busy();
}

auto get_state() {
    StatusRegister<&status_reg_mem> status_reg;
    return status_reg.read().state();
}
