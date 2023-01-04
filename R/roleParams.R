#' @title Parameters of one roleModel
#' @description An S4 class containing RoLE model params
#'
#' @slot individuals_local number of individuals in local community
#' @slot individuals_meta number of individuals in meta community
#' @slot species_meta number of species in meta community
#' @slot speciation_local local speciation probability
#' @slot speciation_meta speciation rate in meta community
#' @slot extinction_meta extinction rate in meta community
#' @slot trait_sigma rate of Brownian trait evolution in the meta community
#' @slot env_sigma selectivity of environmental filter
#' @slot env_optim trait optimum of environmental filter
#' @slot comp_sigma selectivity of competition
#' @slot neut_delta proportion of neutral drift (versus non-neutral)
#' @slot env_delta proportion of filtering (versus competition)
#' @slot dispersal_prob local dispersal probability
#' @slot mutation_rate mutation rate
#' @slot equilib_escape proportion of equilibrium achieved
#' @slot num_basepairs number of basepairs
#' @slot alpha scalar converting from census size to Ne
#'
#' @slot init_type initialization routine; a single character string either
#'     "oceanic_island" or "bridge_island"
#'
#' @slot niter an integer specifying the number of iterations
#' @slot niterTimestep an integer specifying the frequency (in numbers of
#'     iterations) at which the model state is saved
#'
#' @rdname roleParams
#' @export

roleParams <- setClass('roleParams',
                       slots = c(
                           individuals_local = "integer",
                           individuals_meta = "integer",
                           species_meta = "integer",
                           speciation_local = "numeric",
                           speciation_meta = "numeric",
                           extinction_meta = "numeric",
                           trait_sigma = "numeric",
                           env_sigma = "numeric",
                           env_optim = "matrix",
                           comp_sigma = "numeric",
                           neut_delta = "numeric",
                           env_delta = "numeric",
                           dispersal_prob = "numeric",
                           mutation_rate = "numeric" ,
                           equilib_escape = "numeric",
                           alpha = "numeric",
                           num_basepairs = "integer",
                           init_type = "character",
                           niter = 'integer',
                           niterTimestep = 'integer'))



# constructor
#' @rdname roleParams
#' @export

roleParams <- function(individuals_local,
                       individuals_meta,
                       species_meta,
                       speciation_local,
                       speciation_meta,
                       extinction_meta,
                       trait_sigma,
                       env_sigma,
                       env_optim,
                       comp_sigma,
                       neut_delta,
                       env_delta,
                       dispersal_prob,
                       mutation_rate,
                       equilib_escape,
                       alpha,
                       num_basepairs,
                       init_type,
                       niter,
                       niterTimestep) {
    # check that `niter` is given correctly
    if(missing(niter) | length(niter) > 1) {
        stop('must supply a single value for `niter`')
    }

    # check for missing params
    allParams <- as.list(environment())

    withVal <- names(as.list(match.call())[-1])
    noVal <- names(allParams[!(names(allParams) %in% withVal)])

    # loop over params, those that are missing, make them 0
    for(i in 1:length(allParams)) {
        if(names(allParams[i]) %in% noVal) {
            allParams[[i]] <- 0
        }
    }

    # if `init_type` was missing (and now `0`) make it `oceanic_island`
    # as a default
    allParams$init_type <- 'oceanic_island'

    # if `niterTimestep` was missing, make it a meaningful multiple of `niter`
    if(allParams$niterTimestep == 0) {
        if(allParams$niter < 10 * allParams$individuals_local[1]) {
            allParams$niterTimestep <- allParams$niter
        } else {
            allParams$niterTimestep <- 10 * allParams$individuals_local[1]
        }
    }

    return(new('roleParams',
               individuals_local = as.integer(allParams$individuals_local),
               individuals_meta = as.integer(allParams$individuals_meta),
               species_meta = as.integer(allParams$species_meta),
               speciation_local = allParams$speciation_local,
               speciation_meta = allParams$speciation_meta,
               extinction_meta = allParams$extinction_meta,
               trait_sigma = allParams$trait_sigma,
               env_sigma = allParams$env_sigma,
               env_optim = allParams$env_optim,
               neut_delta = allParams$neut_delta,
               env_delta = allParams$env_delta,
               comp_sigma = allParams$comp_sigma,
               dispersal_prob = allParams$dispersal_prob,
               mutation_rate = allParams$mutation_rate,
               equilib_escape = allParams$equilib_escape,
               alpha = allParams$alpha,
               num_basepairs = as.integer(allParams$num_basepairs),
               init_type = allParams$init_type,
               niter = as.integer(allParams$niter),
               niterTimestep = as.integer(allParams$niterTimestep)))
}



