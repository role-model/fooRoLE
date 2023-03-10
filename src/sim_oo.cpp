#include <RcppArmadillo.h>
#include <random>
using namespace Rcpp;
using namespace arma;


// function to calculate comp matrix
mat compMatCalc(mat x, double sigC) {
    // make a matrix to hold distances
    int n = x.n_rows;
    mat D(n, n);
    D.fill(0.0);

    // init a double to hold one dist
    double trtDist;

    // loop over rows and cols to get distances with `norm` function
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            trtDist = norm(x.row(i) - x.row(j));
            D(i, j) = exp(-pow(trtDist / sigC, 2)); // Gaussian comp kernel
            D(j, i) = D(i, j);
        }
    }

    return D;
}

// exposing `compMatCalc` to R for testing purposes
// [[Rcpp::export]]
NumericMatrix compMatCalcTest(NumericMatrix x, double sigC) {
    mat X = as<mat>(x);
    mat m = compMatCalc(X, sigC);

    return wrap(m);
}


// calculate environmental distances
vec envDistCalc(mat x, mat envOptim, double sigE) {
    int n = x.n_rows;
    vec D(n);

    double eDist;

    for (int i = 0; i < n; i++) {
        eDist = norm(x.row(i) - envOptim);
        D(i) = exp(-pow(eDist / sigE, 2));
    }

    return D;
}

// expose `envDistCalc` to R for testing
// [[Rcpp::export]]
NumericVector envDistCalcTest(NumericMatrix x, NumericMatrix envOptim,
                              double sigE) {
    mat X = as<mat>(x);
    mat eo = as<mat>(envOptim);
    // eo = reshape(eo, 1, X.n_cols);

    vec m = envDistCalc(X, eo, sigE);

    return wrap(m);
}


// extract environmental optimum
mat getEnvOptim(S4 x) {
    mat m = as<mat>(x.slot("env_optim"));

    return m;
}

// get a param when we know it will be of class `double`
double getParamDouble(S4 p, String s) {
    Rcout << "extracting param" << s.get_cstring() << "\n";
    double x = as<double>(p.slot(s));

    return x;
}

// function to update phylo objects
// maybe we don't need to return anything, maybe pointers would work?
List updatePhylo(int i, int sMax, imat edge, vec edgeLength,
                 LogicalVector alive) {
    // specPhyloRCpp(int i, int n, NumericMatrix e,
    //               NumericVector l, LogicalVector alive) {

    // index of where unrealized edges in edge matrix start
    int eNew = 2 * sMax - 2;

    // index of the edge matrix of where to add new edge
    uvec inds = find(edge.col(1) == i);
    int j = inds(0);

    // old way
    // int j = -1;
    // for (int k = 0; k < eMax; k++) {
    //     if (e(k, 1) == i) {
    //         j = k;
    //         break;
    //     }
    // }

    // add one to internal nodes
    uvec internalNode = find(edge > sMax); // should it be > or >=??????
    edge.elem(internalNode) += 1;

    // might need to do it for each col separately
    // uvec internal0 = find(edge.col(0) > sMax);
    // uvec internal1 = find(edge.col(1) > sMax);

    // old way
    // for (int r = 0; r < eNew; r++) {
    //     for (int c = 0; c < 2; c++){
    //         if (e(r, c) > n) {
    //             e(r, c) ++;
    //         }
    //     }
    // }


    // add new internal node
    int newNode = 2 * sMax + 1; // index of new node; maybe +1 maybe not?
    edge(eNew, 0) = newNode;
    edge(1 + eNew, 0) = newNode;

    // add tips
    edge(eNew, 1) = edge(j, 1); // add old tip
    edge(eNew + 1, 1) = sMax + 1; // add new tip

    // update ancestry of internal nodes
    edge(j, 1) = newNode;

    // augment edge lengths
    edgeLength[eNew] = 0;
    edgeLength[1 + eNew] = 0;

    // increase all tip edge lengths by 1 time step
    edgeLength(find(edge.col(1) <= eNew + 1)) += 1;

    // old way
    // for (int r = 0; r <= eNew + 1; r++) {
    //     if (e(r, 1) <= n + 1) {
    //         l(r) ++;
    //     }
    // }

    // update sMax
    sMax++;

    // update alive vector
    alive(sMax) = true;

    List out = List::create(Named("edge") = edge,
                            Named("edgeLength") = edgeLength,
                            Named("alive") = alive,
                            Named("sMax") = sMax);

    return out;
}


// class to hold all objects of a role model
// with methods defined for updating those objects
class roleComm {
private:
    std::mt19937 rng; // generator for rand unif nums
    std::uniform_real_distribution<double> dist; // unif dist object
    IntegerVector localSpp; // passed
    mat localTrt; // passed
    NumericVector metaAbund; // passed
    NumericMatrix metaTrt; // passed
    imat edge; // passed
    vec edgeLength; // passed
    LogicalVector alive; // passed
    S4 params; // passed
    int sMax; // passed
    double sigC; // from params.slot("sigC")
    double sigE; // from params.slot("sigE")
    double sig; // from params.slot("trait_sigma")
    double delta; //
    double gamma; //
    double immProb; // from params.slot("imm")
    double specProb; // from params.slot("speciation_local")
    mat envOptim; // from params.slot("envOptim")
    mat compMat; // from localTrt_ and sigC
    vec envDist; // from localTrt_ and sigE

public:
    roleComm(IntegerVector localSpp_,
             mat localTrt_,
             NumericVector metaAbund_,
             NumericMatrix metaTrt_,
             imat edge_,
             vec edgeLength_,
             LogicalVector alive_,
             int sMax_,
             S4 params_) :
    rng(std::mt19937(std::random_device{}())),
    dist(0, 1),
    localSpp(localSpp_),
    localTrt(localTrt_),
    metaAbund(metaAbund_),
    metaTrt(metaTrt_),
    edge(edge_),
    edgeLength(edgeLength_),
    alive(alive_),
    params(params_),
    sMax(sMax_),
    sigC(getParamDouble(params_, "comp_sigma")),
    sigE(getParamDouble(params_, "env_sigma")),
    sig(getParamDouble(params_, "trait_sigma")),
    delta(getParamDouble(params_, "neut_delta")),
    gamma(getParamDouble(params_, "env_delta")),
    immProb(getParamDouble(params_, "dispersal_prob")),
    specProb(getParamDouble(params_, "speciation_local")),
    envOptim(getEnvOptim(params_)),
    compMat(compMatCalc(localTrt_, sigC)),
    envDist(envDistCalc(localTrt_, envOptim, sigE)) {}

    // `get` methods
    List getLocal() {
        IntegerVector spp = as<IntegerVector>(wrap(localSpp));
        NumericMatrix trt = as<NumericMatrix>(wrap(localTrt));

        return List::create(Named("spp") = spp,
                            Named("trt") = trt);
    }

    // method to extract spp ID vec and trt matrix
    List getLocsData() {
        IntegerVector spp = as<IntegerVector>(wrap(localSpp));
        NumericMatrix trt = as<NumericMatrix>(wrap(localTrt));

        return List::create(Named("spp") = spp,
                            Named("trt") = trt);
    }

    // method to extract params
    S4 getParams() {
        return params;
    }

    // get the whole 'foo' class
    List getFoo() {
        List locs = List::create(Named("indSpecies") = localSpp,
                            Named("indTrait") = wrap(localTrt));

        List meta = List::create(Named("sppAbund") = metaAbund,
                                 Named("sppTrait") = metaTrt);

        List phylo = List::create(Named("n") = sMax,
                                  Named("e") = wrap(edge),
                                  Named("l") = wrap(edgeLength),
                                  Named("alive") = alive);

        List out = List::create(Named("localComm") = locs,
                                Named("metaComm") = meta,
                                Named("phylo") = phylo);

        return out;
    }

    // process methods
    int death() {
        // index of the dead individual
        int idead;

        if (delta == 1) {
            Rcout << "in neutral death \n";
            // fully neutral
            idead = sample(localSpp.size(), 1)[0] - 1;
        } else {
            // set up vectors to hold death probs
            vec compD;
            vec probs;

            // gamma determines amount of env filtering v. comp
            if (gamma < 1) {
                // competition calcs
                // the `compMat` is symmetric, so fastest way to sum is by col
                vec cd = sum(compMat, 0);
            } else {
                // set competition term to 0
                compD = zeros(compMat.n_cols);
            }

            // non-neutral death probabilities
            probs = gamma * envDist + (1 - gamma) * compD;

            // index of dead individual
            idead = sample(localSpp.size(), 1, false, wrap(probs))[0] - 1;
        }

        // return the index of the dead individual so it can be replaced
        return idead;
    }



    void birthImm(int i) {
        // set up indexes
        int inew;
        int iborn;

        // random number to determine which event occurs
        double r = dist(rng);

        // row vec to hold new trait, initialized with random noise
        rowvec newTrt = randn<rowvec>(localTrt.n_cols) * sig *
            2 / localSpp.size(); // re-scale by generation time

        if (r < immProb) { // immigration
            // sample the spp ID of the immigrating individual
            inew = sample(metaAbund.size(), 1, false, metaAbund)[0] - 1;

            // update traits from meta comm
            newTrt += metaTrt.row(inew);
        } else { // local birth
            // sample the individual that gives birth
            iborn = sample(localSpp.size(), 1)[0] - 1;

            // spp ID of individual that gave birth
            inew = localSpp[iborn];

            // update traits from local comm
            newTrt += localTrt.row(inew);
        }

        // update local comm spp ID
        localSpp[i] = inew;

        // update traits
        localTrt.row(i) = newTrt;
    }

    void speciation(int i) {
        // random number to determine if speciation happens
        double r = dist(rng);

        if (r < specProb) {
            // update phylo
            Rcout << "got to speciation method \n";

            List newPhyInfo = updatePhylo(i, sMax, edge, edgeLength, alive);
            edge = as<imat>(newPhyInfo["edge"]); // not ideal that we have to cast these things with as<type>
            edgeLength = as<vec>(newPhyInfo["edgeLength"]);
            alive = newPhyInfo["alive"];

            // update total number of spp
            sMax = newPhyInfo["sMax"];

            // update ID of local individual
            localSpp[i] = sMax;

            // update traits
            rowvec newTrt = localTrt.row(i) +
                randn<rowvec>(localTrt.n_cols) * sig; // could re-scale lineage duration
        }
    }

    void updateDist(int i) {
        Rcout << "got to updateDist method \n";
        // only need to update distances if we're in a non-neutral sim
        if (delta < 1) {
            // update comp distances
            // note: `envDistCalc` can be used because we're calculating distances
            //        between 1 ind (= the "optimum") and everybody else
            vec newComp = envDistCalc(localTrt, localTrt.row(i), sigC);
            compMat.col(i) = newComp;
            compMat.row(i) = newComp.t();

            // update env dist
            envDist.row(i) = envDistCalc(localTrt.row(i), envOptim, sigE);
        }
    }
};


// function that takes a roleFoo S4 object and creates a roleComm rcpp object
roleComm roleCommFromS4(S4 x) {
    // local comm stuff
    S4 locs = x.slot("localComm");
    IntegerVector localSpp_ = locs.slot("indSpecies");
    mat localTrt_ = as<mat>(locs.slot("indTrait"));

    // meta comm stuff
    S4 meta = x.slot("metaComm");
    NumericVector metaAbund_ = meta.slot("sppAbund");
    NumericMatrix metaTrt_ = meta.slot("sppTrait");

    // phylo stuff
    Rcout << "got to extracting phylo \n";
    S4 phy = x.slot("phylo");
    imat edge_ = as<imat>(phy.slot("e"));
    vec edgeLength_ = as<vec>(phy.slot("l"));
    LogicalVector alive_ = phy.slot("alive");
    int sMax_ = as<int>(phy.slot("n"));

    Rcout << "got through extracting phylo \n";

    // params
    S4 params_ = x.slot("params");

    roleComm out = roleComm(localSpp_,
                            localTrt_,
                            metaAbund_,
                            metaTrt_,
                            edge_,
                            edgeLength_,
                            alive_,
                            sMax_,
                            params_);

    return out;
}


// [[Rcpp::export]]
List BigOlTester(S4 x) {
    roleComm wow = roleCommFromS4(x);

    List l = List::create(Named("locs") = wow.getLocal(),
                          Named("pzz") = wow.getParams());
    return l;
}

// OO version of simulation function
// [[Rcpp::export]]
List simOO(S4 x) {
    // consider alternatives to clone????
    x = clone(x);
    roleComm wow = roleCommFromS4(x);

    // get params
    S4 p = wow.getParams();

    // number of iterations and output timesteps
    int niter = as<int>(p.slot("niter"));
    int niterTimestep = as<int>(p.slot("niterTimestep"));
    int n = niter / niterTimestep + 1; // number of output values
    int k; // index for filling in output list `l`

    // list to hold output
    List l(n);

    // record initial state
    l[0] = clone(wow.getFoo());

    // main loop of sim---starts at 1 because we already recorded the
    // initial state
    for (int i = 1; i <= niter; i++) {
        // death
        int idead = wow.death();

        // immigration or local birth
        wow.birthImm(idead);

        // speciation or not
        wow.speciation(idead);

        // udate distances
        wow.updateDist(idead);

        // every `niterTimestep`, record state
        if (i % niterTimestep == 0) {
            k = i / niterTimestep;
            l[k] = clone(wow.getFoo());
        }
    }

    return l;
}


// classes to try to understand updating and behavior of pointers for use in
// speciation methods

// a class where the matrix updates are all internal
class allIn {
private:
    imat edge;
    int sMax;

public:
    allIn(imat edge, int sMax) : edge(edge), sMax(sMax) {}

    void speciation() {
        uvec i = find(edge.col(1) < sMax) + edge.n_rows;
        edge(i) += 100;

        sMax++;
    }

    IntegerMatrix getEdge() {
        return wrap(edge);
    }

    int getS() {
        return sMax;
    }
};



// [[Rcpp::export]]
List testAllIn(S4 x) {
    x = clone(x);

    imat e = as<imat>(x.slot("e"));
    int s = as<int>(x.slot("n"));

    allIn obj = allIn(e, s);

    obj.speciation();

    List l = List::create(Named("e") = obj.getEdge(),
                          Named("s") = obj.getS());

    return l;
}

// a class where the matrix updates are done by an external function that
// gets passed arguments by referece

void specFun(imat& edge, int& sMax) {
    uvec i = find(edge.col(1) < sMax) + edge.n_rows;
    edge(i) += 100;

    sMax++;
}


class allOut {
private:
    imat edge;
    int sMax;

public:
    allOut(imat edge, int sMax) : edge(edge), sMax(sMax) {}

    void speciation() {
        specFun(edge, sMax);
    }

    IntegerMatrix getEdge() {
        return wrap(edge);
    }

    int getS() {
        return sMax;
    }
};



// [[Rcpp::export]]
List testAllOut(S4 x) {
    x = clone(x);

    imat e = as<imat>(x.slot("e"));
    int s = as<int>(x.slot("n"));

    allOut obj = allOut(e, s);

    obj.speciation();

    List l = List::create(Named("e") = obj.getEdge(),
                          Named("s") = obj.getS());

    return l;
}
