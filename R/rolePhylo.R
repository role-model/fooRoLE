#' @title RoLE phylogeny
#'
#' @description An S4 class to specify a phylogeny in a way that can be easily
#'     evolved through simulation
#'
#' @slot n number of tips
#' @slot e edge matrix; two columns give ancestor, child pair
#' @slot l numeric vector of edge lengths (in units of time steps = 1/J
#'     generations)
#' @slot alive vector indicating whether tips are extant or not
#' @slot tipNames names of tips
#'
#' @export

setClass('rolePhylo',
         slots = c(n = 'integer',
                   e = 'matrix',
                   l = 'numeric',
                   alive = 'logical',
                   tipNames = 'character'))


#' @title Specify a RoLE model phylogeny
#'
#' @param n number of tips
#' @param e edge matrix; two columns give ancestor, child pair
#' @param l numeric vector of edge lengths
#' @param alive logical vector indicating whether tips are extant or not
#' @param tipNames character vector of tip names
#'
#' @export

rolePhylo <- function(n, e, l, alive, tipNames) {
    new('rolePhylo',
        n = n, e = e, l = l, alive = alive, tipNames = tipNames)
}



# set coercion method from ape::phylo to roleR::rolePhylo
setAs(from = 'phylo', to = 'rolePhylo',
      def = function(from) {
          # extract number of times
          n <- ape::Ntip(from)

          # extract edge matrix and edge lengths
          e <- from$edge
          l <- from$edge.length

          # extract tip labels
          tipNames <- from$tip.label

          # calculate alive or not
          tipAge <- ape::node.depth.edgelength(from)[1:n]

          alive <- rep(TRUE, n)
          alive[tipAge < max(tipAge)] <- FALSE


          # set default scale
          scale <- 1

          return(rolePhylo(n = n, e = e, l = l, alive = alive,
                           tipNames = tipNames))
      }
)


# set coercion method from roleR::rolePhylo to ape::phylo
setAs(from = 'rolePhylo', to = 'phylo',
      def = function(from) {
          i <- 2 * (from@n - 1)

          y <- list(edge = from@e[1:i, ], edge.length = from@l[1:i],
                    tip.label = from@tipNames[1:from@n],
                    Nnode = from@n - 1)

          # make any possible 0 or negative edge lengths equal to
          # very small number
          y$edge.length[y$edge.length <= 0] <- .Machine$double.eps

          class(y) <- 'phylo'

          return(y)
      }
)
