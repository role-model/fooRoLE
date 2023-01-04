#' @title Local community
#'
#' @description An S4 class to specify the state of a local community
#'
#' @slot indSpecies integer vector of spp IDs
#' @slot indTrait numeric matrix of trait values for each individual (= rows)
#' @slot indSeqs character vector with sequences for each individual
#'
#' @rdname localComm
#' @export

setClass('localComm',
         slots = c(indSpecies = 'integer',
                   indTrait = 'matrix',
                   indSeqs = 'character'))


# constructor
#' @rdname localComm
#' @export

localComm <- function(indSpecies, indTrait, indSeqs) {

    return(new('localComm',
             indSpecies = indSpecies,
             indTrait = indTrait,
             indSeqs = indSeqs))
}

