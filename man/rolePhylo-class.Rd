% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/rolePhylo.R
\docType{class}
\name{rolePhylo-class}
\alias{rolePhylo-class}
\title{RoLE phylogeny}
\description{
An S4 class to specify a phylogeny in a way that can be easily
    evolved through simulation
}
\section{Slots}{

\describe{
\item{\code{n}}{number of tips}

\item{\code{e}}{edge matrix; two columns give ancestor, child pair}

\item{\code{l}}{numeric vector of edge lengths (in units of time steps = 1/J
generations)}

\item{\code{alive}}{vector indicating whether tips are extant or not}

\item{\code{tipNames}}{names of tips}
}}

