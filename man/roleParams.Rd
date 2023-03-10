% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/roleParams.R
\name{roleParams}
\alias{roleParams}
\title{Parameters of one roleModel}
\usage{
roleParams(
  individuals_local,
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
  niterTimestep
)

roleParams(
  individuals_local,
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
  niterTimestep
)
}
\description{
An S4 class containing RoLE model params
}
\section{Slots}{

\describe{
\item{\code{individuals_local}}{number of individuals in local community}

\item{\code{individuals_meta}}{number of individuals in meta community}

\item{\code{species_meta}}{number of species in meta community}

\item{\code{speciation_local}}{local speciation probability}

\item{\code{speciation_meta}}{speciation rate in meta community}

\item{\code{extinction_meta}}{extinction rate in meta community}

\item{\code{trait_sigma}}{rate of Brownian trait evolution in the meta community}

\item{\code{env_sigma}}{selectivity of environmental filter}

\item{\code{env_optim}}{trait optimum of environmental filter}

\item{\code{comp_sigma}}{selectivity of competition}

\item{\code{neut_delta}}{proportion of neutral drift (versus non-neutral)}

\item{\code{env_delta}}{proportion of filtering (versus competition)}

\item{\code{dispersal_prob}}{local dispersal probability}

\item{\code{mutation_rate}}{mutation rate}

\item{\code{equilib_escape}}{proportion of equilibrium achieved}

\item{\code{num_basepairs}}{number of basepairs}

\item{\code{alpha}}{scalar converting from census size to Ne}

\item{\code{init_type}}{initialization routine; a single character string either
"oceanic_island" or "bridge_island"}

\item{\code{niter}}{an integer specifying the number of iterations}

\item{\code{niterTimestep}}{an integer specifying the frequency (in numbers of
iterations) at which the model state is saved}
}}

