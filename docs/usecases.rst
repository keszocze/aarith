Use Cases
=========

In the following, use cases for Aarith will be shown. (So far, we only present one)

.. _fau_adder:

The FAU Adder
-------------

The FAU Adder (see [Echavarria2016]_) splits the operands into a most-significant part (MSP) and least-significant part
(LSP) and cuts the carry chain in between these parts. To reduce the error, some bits of the LSP (``shared_bits`` in total)
are used the predict the carry.

The following code implements the FAU adder. This piece of code is intended to show how easily individual bits ca
be accessed when designing hardware units.


.. code-block:: c++
    :linenos:

    template <size_t width, size_t lsp_width, size_t shared_bits = 0>
    uinteger<width + 1> FAUadder(const uinteger<width>& a, const uinteger<width>& b)
    {

        // make sure that the parameters
        static_assert(shared_bits <= lsp_width);
        static_assert(lsp_width < width);
        static_assert(lsp_width > 0);

        //***********************
        // Extract MSP and LSP
        // can't use structured binding (i.e. `` auto [msp, lsp] = split<lsp_index>(a)``) as msp and lsp need
        // not bee of same width
        constexpr size_t lsp_index = lsp_width - 1;
        const auto a_split = split<lsp_index>(a);
        const auto b_split = split<lsp_index>(b);

        const uinteger<lsp_width> a_lsp = a_split.second;
        const uinteger<lsp_width> b_lsp = b_split.second;

        constexpr size_t msp_width = width - lsp_width;
        const uinteger<msp_width> a_msp = a_split.first;
        const uinteger<msp_width> b_msp = b_split.first;

        // sum up LSP including the computation of the carry
        uinteger<lsp_width + 1> lsp_sum = expanding_add(a_lsp, b_lsp);

        // remove the carry for later use
        uinteger<lsp_width> lsp = width_cast<lsp_width>(lsp_sum);

        bool predicted_carry = false;
        // conditionally perform carry prediction
        if constexpr (shared_bits > 0)
        {
            // extract the shared bit of both operands
            uinteger<shared_bits> a_shared = bit_range<lsp_index, lsp_index - (shared_bits - 1)>(a);
            uinteger<shared_bits> b_shared = bit_range<lsp_index, lsp_index - (shared_bits - 1)>(b);

            // compute the carry
            uinteger<shared_bits + 1> shared_sum = expanding_add(a_shared, b_shared);

            predicted_carry = shared_sum.msb();
        }

        // if there was a carry but we did not predict one (i.e. it wasn't used in the MSP)
        // we need to perform an all1 error correction
        if (lsp_sum.msb() && !predicted_carry)
        {
            lsp = lsp.all_ones();
        }

        // finally put MSP and LSP together
        const uinteger<msp_width + 1> msp = expanding_add(a_msp, b_msp, predicted_carry);

        uinteger<width + 1> result{lsp};

        const auto extended_msp = width_cast<width + 1>(msp);
        result = add(result, extended_msp << lsp_width);
        return result;
    }