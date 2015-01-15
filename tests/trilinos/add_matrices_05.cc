// ---------------------------------------------------------------------
//
// Copyright (C) 2004 - 2015 by the deal.II authors
//
// This file is part of the deal.II library.
//
// The deal.II library is free software; you can use it, redistribute
// it, and/or modify it under the terms of the GNU Lesser General
// Public License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
// The full text of the license can be found in the file LICENSE at
// the top level of the deal.II distribution.
//
// ---------------------------------------------------------------------



// check SparseMatrix::add(SparseMatrix) where the other matrix has either
// more or less entries (but with reasonable entries in any case)

#include "../tests.h"
#include <deal.II/base/utilities.h>
#include <deal.II/lac/trilinos_sparse_matrix.h>
#include <fstream>
#include <iostream>


void test (TrilinosWrappers::SparseMatrix &m)
{
  TrilinosWrappers::SparseMatrix m2(m.m(), m.n(), 0);

  // first set a few entries one-by-one
  for (unsigned int i=0; i<m.m(); ++i)
    for (unsigned int j=0; j<m.n(); ++j)
      if ((i+2*j+1) % 3 == 0)
        {
          m.set (i,j, i*j*.5+.5);
          m2.set (i,j, 1.);
        }
      else if (j % 2 == 0)
        m2.set(i,j, 0);

  m.compress (VectorOperation::insert);
  m2.compress(VectorOperation::insert);

  deallog << "Matrix nonzeros: " << m.n_nonzero_elements() <<  " "
          << m2.n_nonzero_elements() << std::endl;

  m.print(deallog.get_file_stream());
  deallog << std::endl;

  m.add(1, m2);
  m.print(deallog.get_file_stream());
  deallog << std::endl;

  m.add(-1, m2);
  m2.add(-1, m);
  m2.print(deallog.get_file_stream());
  deallog << std::endl;

  m.add(-1, m2);
  m.print(deallog.get_file_stream());

  deallog << "OK" << std::endl;
}



int main (int argc,char **argv)
{
  std::ofstream logfile("output");
  deallog.attach(logfile);
  deallog.depth_console(0);
  deallog.threshold_double(1.e-10);

  Utilities::MPI::MPI_InitFinalize mpi_initialization (argc, argv, numbers::invalid_unsigned_int);

  try
    {
      {
        TrilinosWrappers::SparseMatrix m (5U,6U,3U);

        test (m);
      }
    }
  catch (std::exception &exc)
    {
      std::cerr << std::endl << std::endl
                << "----------------------------------------------------"
                << std::endl;
      std::cerr << "Exception on processing: " << std::endl
                << exc.what() << std::endl
                << "Aborting!" << std::endl
                << "----------------------------------------------------"
                << std::endl;

      return 1;
    }
  catch (...)
    {
      std::cerr << std::endl << std::endl
                << "----------------------------------------------------"
                << std::endl;
      std::cerr << "Unknown exception!" << std::endl
                << "Aborting!" << std::endl
                << "----------------------------------------------------"
                << std::endl;
      return 1;
    };
}
