#ifndef SRC_ACTIONS_HPP_
#define SRC_ACTIONS_HPP_

struct Action {
    unsigned input_n;
    unsigned output_n;

    void (*f)(void*);
};

#endif // SRC_ACTIONS_HPP_
