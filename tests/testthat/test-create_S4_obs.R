library(fooRoLE)

# test creation of S4 classes

locs <- localComm(rep(1L, 4), matrix(1.1, nrow = 4, ncol = 1), rep('A', 4))

meta <- metaComm(rep(1, 3), matrix(1:3, ncol = 1))

phy <- as(ape::rphylo(3, 1, 0.8), 'rolePhylo')

p <- roleParams(individuals_local = 4,
                individuals_meta = 4,
                species_meta = 3,
                env_sigma = 1.1,
                comp_sigma = 2,
                dispersal_prob = 0.5,
                niter = 10)

foo <- roleFoo(locs, meta, phy, p)


test_that('`localComm` works', {
    expect_s4_class(locs, 'localComm')
})

test_that('`metaComm` works', {
    expect_s4_class(meta, 'metaComm')
})

test_that('`rolePhylo` works', {
    expect_s4_class(phy, 'rolePhylo')
})

test_that('`roleParams` works', {
    expect_s4_class(p, 'roleParams')
})

test_that('`roleFoo` works', {
    expect_s4_class(foo, 'roleFoo')
})
