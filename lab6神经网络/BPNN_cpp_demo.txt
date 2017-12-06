/*

BPNN CPP code frame.
Only for reference.

*/

void initialize_weight( type weight )
{
    // type is up to you

    // initialize the weight array for every input node to hidden node
    ...

    // initialize the weight array for every hidden node to output node
    ...
}

type forward_pass_i2h( type input_nodes, type weight_i2h )
{
    // type is up to you

    // use sigmoid function to calculate output for hidden layer input
    ...
}

type forward_pass_h2o( type hidden_nodes, type weight_h2o )
{
    // type is up to you
    // calculate output for output layer
    ...
}

type backward_pass(type output, type weight)
{
    // this function should update all weights
    // or you can split this function into two functions, updating different parts of weights


    // calculate output errors
    ...

    // propagate the error to the hidden layer
    ...

    // calculate hidden layer gradient
    ...

    // update weight
    ...
}


int main()
{
    initialize_weight( weight );

    while( stopping condition not met ) {
        // calculate input for hidden layer, using sigmoid function
        // i2h => input to hidden
        output_i2h = forward_pass_i2h( input_nodes, weight_i2h );
        
        // calculate the output for output layer
        // h2o => hidden to output
        output_h2o = forward_pass_h2o( hidden_nodes, weight_h2o );

        // calculate error for update
        backward_pass( output_h2o, weight );
    }

    test_data();

    return 0;
}