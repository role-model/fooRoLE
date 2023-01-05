library(fooRoLE)


# test calculation of competitive matrix and env distances

m <- matrix(runif(12), nrow = 4)

compMatCalcTest(m, 2)


envDistCalcTest(m, matrix(0, nrow = 1, ncol = ncol(m)), 1.5)



# test creation of S4 classes

locs <- localComm(rep(1L, 4), matrix(1.1, nrow = 4, ncol = 1), rep('A', 4))

meta <- metaComm(rep(1, 3), matrix(1:3, ncol = 1))

phy <- as(ape::rphylo(3, 1, 0.8), 'rolePhylo')

p <- roleParams(individuals_local = 4,
                individuals_meta = 4,
                species_meta = 3,
                trait_sigma = 1,
                env_sigma = 1.1,
                env_optim = matrix(0),
                comp_sigma = 2,
                neut_delta = 1,
                dispersal_prob = 0.5,
                niter = 2,
                niterTimestep = 1)

foo <- roleFoo(locs, meta, phy, p)


# test passing and receiving S4 objects

BigOlTester(foo)


# test oo sim

maybe <- simOO(foo)


# testing the right way to index armadillo matrices for phylo updating

# also testing passing args by reference for updating
# take home: we should pass by reference for updating phylo!

foo@phylo@n
foo@phylo@e
testAllIn(foo@phylo)
foo@phylo@n
foo@phylo@e

testAllOut(foo@phylo)
