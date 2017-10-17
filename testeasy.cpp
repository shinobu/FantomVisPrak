#include <fantom/algorithm.hpp>
#include <fantom/register.hpp>
#include <fantom/fields.hpp>
using namespace fantom;
namespace
{
    class CommitTutorialAlgorithm : public DataAlgorithm
    {
    public:
        struct Options : public DataAlgorithm::Options
        {
            Options( fantom::Options::Control& control )
                : DataAlgorithm::Options( control )
            {
            }
        };

        //================================================================================================================

        struct DataOutputs : public DataAlgorithm::DataOutputs
        {
            DataOutputs( fantom::DataOutputs::Control& control )
                : DataAlgorithm::DataOutputs( control )
            {
                add< Grid< 3 > >( "grid" );
            }
        };

        //================================================================================================================

        CommitTutorialAlgorithm( InitData& data )
            : DataAlgorithm( data )
        {
        }

        //================================================================================================================

        virtual void execute( const Algorithm::Options& options, const volatile bool& /*abortFlag*/ ) override
        {
          // ##### Variable #####
          size_t t= (size_t) 3;
          //const std::pair< Cell::Type, size_t >  cellCounts =
          //std::unique_ptr< ValueArray< size_t > >  	indices =
          // ##### Domain #####
            long nx = 10;
            long ny = 10;
            long nz = 10;
            double dx = 1.0;
            double dy = 1.0;
            double dz = 1.0;
            size_t extent[] = { (size_t)nx,
                                (size_t)ny,
                                (size_t)nz };
            double origin[] = { -0.5 * dx * ( nx - 1 ),
                                -0.5 * dy * ( ny - 1 ),
                                -0.5 * dz * ( nz - 1 ) };
            double spacing[] = { dx, dy, dz };

            std::shared_ptr< const DiscreteDomain< 3 > > mDomain = DomainFactory::makeDomainUniform( extent, origin, spacing );

            //##### Make Object #####
            std::shared_ptr< const Grid< 3 > > mGrid = DomainFactory::makeGridStructured( *mDomain );
            setResult( "grid", mGrid );

            //##### New Object #####
            //static std::shared_ptr< const Grid< D > > fantom::DomainFactory::makeGridUnstructured(*mDomain, t, cellCounts, indices)


        }
    };
    AlgorithmRegister< CommitTutorialAlgorithm > dummy( "Tutorial/Commit2", "Generate a simple vector field." );
}
