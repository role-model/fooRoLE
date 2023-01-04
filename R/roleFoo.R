#' @title A set of objects representing the state of a roleModel
#' @description  Contains community states, phylo state, and params
#' @slot localComm an object of class \code{localComm}
#' @slot metaComm an object of class \code{metaComm}
#' @slot phylo an object of class \code{rolePhylo}
#' @slot params an object of class \code{roleParams}
#'
#' @export

setClass('roleFoo',
         slots = c(localComm = 'localComm',
                   metaComm = 'metaComm',
                   phylo = 'rolePhylo',
                   params = 'roleParams'))




# constructor
#' @rdname roleFoo
#' @export

roleFoo <- function(localComm, metaComm, phylo, params) {
    return(new('roleFoo',
               localComm = localComm,
               metaComm = metaComm,
               phylo = phylo,
               params = params))
}
