#' @title Meta community
#'
#' @description An S4 class to specify the state of a meta community
#'
#' @slot sppAbund numeric vector with abundances for each spp
#' @slot sppTrait numeric matrix with traits for each spp
#'
#' @rdname metaComm
#' @export

setClass('metaComm',
         slots = c(sppAbund = 'numeric',
                   sppTrait = 'matrix'))


# constructor
#' @rdname metaComm
#' @export

metaComm <- function(sppAbund, sppTrait) {
  return(new('metaComm',
             sppAbund = sppAbund,
             sppTrait = sppTrait))
}
